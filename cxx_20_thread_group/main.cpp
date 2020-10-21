#include <boost/atomic/atomic.hpp>

#include <emmintrin.h>  // for _mm_pause
#include <sched.h>      // for sched_yield
#include <time.h>       // for timespec/nanosleep

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <array>
#include <concepts>
#include <iostream>
#include <mutex>
#include <new>
#include <optional>
#include <queue>
#include <ranges>
// #include <semaphore>
#include <thread>
#include <vector>

// Cache line alignment
#if defined(__i386__) || defined(__x86_64__)
#define CACHELINE_SIZE 64
#elif defined(__powerpc64__)
// TODO(dougkwan) This is the L1 D-cache line size of our Power7 machines.
// Need to check if this is appropriate for other PowerPC64 systems.
#define CACHELINE_SIZE 128
#elif defined(__arm__)
// Cache line sizes for ARM: These values are not strictly correct since
// cache line sizes depend on implementations, not architectures.  There
// are even implementations with cache line sizes configurable at boot
// time.
#if defined(__ARM_ARCH_5T__)
#define CACHELINE_SIZE 32
#elif defined(__ARM_ARCH_7A__)
#define CACHELINE_SIZE 64
#endif
#endif

#ifndef CACHELINE_SIZE
// A reasonable default guess.  Note that overestimates tend to waste more
// space, while underestimates tend to waste more time.
#define CACHELINE_SIZE 64
#endif

#define CACHELINE_ALIGNED __attribute__((aligned(CACHELINE_SIZE)))

namespace taetl
{
template<ptrdiff_t least_max_value>
class counting_semaphore
{
public:
    static constexpr ptrdiff_t max() noexcept { return least_max_value; }

    constexpr explicit counting_semaphore(ptrdiff_t desired) : counter(desired)
    {
        assert(desired >= 0);
        assert(desired <= max());
    }

    ~counting_semaphore();

    counting_semaphore(const counting_semaphore&) = delete;
    counting_semaphore& operator=(const counting_semaphore&) = delete;

    void release(ptrdiff_t update = 1)
    {
        assert(update >= 0);
        assert(update <= max() - counter);
        counter.fetch_add(update, boost::memory_order_acquire);
    }

    void acquire()
    {
        while (!try_acquire())
            ;
    }

    // Returns: true if counter was decremented, otherwise false
    bool try_acquire() noexcept
    {
        if (counter.load() == 0)
        {
            return false;
        }

        counter.fetch_sub(1, boost::memory_order_acquire);
        return true;
    }

    // template <class Rep, class Period>
    // bool try_acquire_for(const chrono::duration<Rep, Period> &rel_time);
    // template <class Clock, class Duration>
    // bool try_acquire_until(const chrono::time_point<Clock, Duration>
    // &abs_time);

private:
    boost::atomic<ptrdiff_t> counter;  // exposition only
};
}  // namespace taetl

struct ticket_mutex
{
public:
    auto lock() -> void
    {
        auto const my = in.fetch_add(1, boost::memory_order_acquire);
        while (true)
        {
            auto const now = out.load(boost::memory_order_acquire);
            if (now == my)
            {
                return;
            }

            // out.wait(now, boost::memory_order_relaxed);
        }
    }

    auto unlock() -> void
    {
        out.fetch_add(1, boost::memory_order_release);
        // out.notify_all();
    }

private:
    alignas(CACHELINE_SIZE) boost::atomic<int> in  = ATOMIC_VAR_INIT(0);
    alignas(CACHELINE_SIZE) boost::atomic<int> out = ATOMIC_VAR_INIT(0);
};

struct spin_mutex
{
    void lock() noexcept
    {
        // approx. 5x5 ns (= 25 ns), 10x40 ns (= 400 ns), and 3000x350 ns
        // (~ 1 ms), respectively, when measured on a 2.9 GHz Intel i9
        constexpr auto iterations = std::array {5, 10, 3000};

        for (int i = 0; i < iterations[0]; ++i)
        {
            if (try_lock()) return;
        }

        for (int i = 0; i < iterations[1]; ++i)
        {
            if (try_lock()) return;

            _mm_pause();
        }

        while (true)
        {
            for (int i = 0; i < iterations[2]; ++i)
            {
                if (try_lock()) return;

                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
                _mm_pause();
            }

            // waiting longer than we should, let's give other threads
            // a chance to recover
            std::this_thread::yield();
        }
    }

    bool try_lock() noexcept { return !flag.test_and_set(boost::memory_order_acquire); }

    void unlock() noexcept { flag.clear(boost::memory_order_release); }

private:
    boost::atomic_flag flag = BOOST_ATOMIC_FLAG_INIT;
};

class thread_group
{
public:
    thread_group(std::uint64_t n, std::invocable<std::stop_token> auto&& f)
    {
        for (auto i = 0u; i < n; i++) members.emplace_back(f);
    }

private:
    std::vector<std::jthread> members;
};

template<typename Type, std::uint64_t QueueDepth>
class concurrent_bounded_queue
{
public:
    auto enqueue(std::convertible_to<Type> auto&& u) -> void
    {
        // remaining_space.aquire();
        push(std::forward<decltype(u)>(u));
        // items_produced.release();
    }

    auto dequeue() -> Type
    {
        // items_produced.aquire();
        Type tmp = pop();
        // remaining_space.release();
        return tmp;
    }

    auto try_dequeue() -> std::optional<Type>
    {
        // if (items_produced.try_aquire()) return {};
        Type tmp = pop();
        // remaining_space.release();
        return tmp;
    }

private:
    auto push(std::convertible_to<Type> auto&& u) -> void
    {
        std::scoped_lock l(items_mtx);
        items.emplace(std::forward<decltype(u)>(u));
    }

    auto pop() -> Type
    {
        std::optional<Type> tmp;
        std::scoped_lock l(items_mtx);
        tmp = std::move(items.front());
        items.pop();
        return std::move(*tmp);
    }

private:
    std::queue<Type> items;
    spin_mutex items_mtx;
    //   std::counting_semaphore<QueueDepth> items_produced{0};
    //   std::counting_semaphore<QueueDepth> remaining_space{QueueDepth};
};

auto main() -> int
{
    boost::atomic<std::uint64_t> count {0};
    {
        thread_group tg(6, [&](std::stop_token s) {
            while (!s.stop_requested())
            {
                ++count;
            }
        });
    }

    std::printf("%ld\n", count.load());

    auto queue = concurrent_bounded_queue<int, 5> {};

    queue.enqueue(5);
    std::cout << queue.dequeue() << '\n';
    std::cout << *queue.try_dequeue() << '\n';

    return EXIT_SUCCESS;
}