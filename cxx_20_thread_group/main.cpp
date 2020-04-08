#include <cassert>
#include <cstdint>
#include <cstdlib>

#include <atomic>
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

#include <sched.h> // sched_yield
#include <time.h>  // timespec/nanosleep

namespace taetl {
template <ptrdiff_t least_max_value> class counting_semaphore {
public:
  static constexpr ptrdiff_t max() noexcept { return least_max_value; }

  constexpr explicit counting_semaphore(ptrdiff_t desired) : counter(desired) {
    assert(desired >= 0);
    assert(desired <= max());
  }

  ~counting_semaphore();

  counting_semaphore(const counting_semaphore &) = delete;
  counting_semaphore &operator=(const counting_semaphore &) = delete;

  void release(ptrdiff_t update = 1) {
    assert(update >= 0);
    assert(update <= max() - counter);
    counter.fetch_add(update, std::memory_order_acquire);
  }

  void acquire() {
    while (!try_acquire())
      ;
  }

  // Returns: true if counter was decremented, otherwise false
  bool try_acquire() noexcept {
    if (counter.load() == 0) {
      return false;
    }

    counter.fetch_sub(1, std::memory_order_acquire);
    return true;
  }

  // template <class Rep, class Period>
  // bool try_acquire_for(const chrono::duration<Rep, Period> &rel_time);
  // template <class Clock, class Duration>
  // bool try_acquire_until(const chrono::time_point<Clock, Duration>
  // &abs_time);

private:
  std::atomic<ptrdiff_t> counter; // exposition only
};
} // namespace taetl

struct ticket_mutex {
public:
  auto lock() -> void {
    auto const my = in.fetch_add(1, std::memory_order_acquire);
    while (true) {
      auto const now = out.load(std::memory_order_acquire);
      if (now == my) {
        return;
      }

      // out.wait(now, std::memory_order_relaxed);
    }
  }

  auto unlock() -> void {
    out.fetch_add(1, std::memory_order_release);
    // out.notify_all();
  }

private:
  // alignas(std::hardware_destructive_interference_size)
  std::atomic<int> in = ATOMIC_VAR_INIT(0);
  std::atomic<int> out = ATOMIC_VAR_INIT(0);
};

struct spin_mutex {
public:
  auto lock() -> void {
    // while (flag.test_and_set(std::memory_order_acquire)) {
    //   flag.wait(true, std::memory_order_relaxed);
    // }

    for (std::uint64_t k = 0; flag.test_and_set(std::memory_order_acquire);
         ++k) {
      if (k < 16) {
        __asm__ __volatile__("rep; nop" : : : "mmory");
      } else if (k < 64) {
        sched_yield();
      } else {
        timespec rqtp = {0, 0};
        rqtp.tv_sec = 0;
        rqtp.tv_nsec = 1000;
        nanosleep(&rqtp, nullptr);
      }
    }
  }

  auto unlock() -> void {
    flag.clear(std::memory_order_release);
    // flag.notify_one();
  }

private:
  std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

class thread_group {
public:
  thread_group(std::uint64_t n, std::invocable<std::stop_token> auto &&f) {
    for (auto i = 0u; i < n; i++)
      members.emplace_back(f);
  }

private:
  std::vector<std::jthread> members;
};

template <typename Type, std::uint64_t QueueDepth>
class concurrent_bounded_queue {
public:
  auto enqueue(std::convertible_to<Type> auto &&u) -> void {
    // remaining_space.aquire();
    push(std::forward<decltype(u)>(u));
    // items_produced.release();
  }

  auto dequeue() -> Type {
    // items_produced.aquire();
    Type tmp = pop();
    // remaining_space.release();
    return std::move(tmp);
  }

  auto try_dequeue() -> std::optional<Type> {
    // if (items_produced.try_aquire()) return {};
    Type tmp = pop();
    // remaining_space.release();
    return std::move(tmp);
  }

private:
  auto push(std::convertible_to<Type> auto &&u) -> void {
    std::scoped_lock l(items_mtx);
    items.emplace(std::forward<decltype(u)>(u));
  }

  auto pop() -> Type {
    std::optional<Type> tmp;
    std::scoped_lock l(items_mtx);
    tmp = std::move(items.front());
    items.pop();
    return std::move(*tmp);
  }

private:
  std::queue<Type> items;
  std::mutex items_mtx;
  //   std::counting_semaphore<QueueDepth> items_produced{0};
  //   std::counting_semaphore<QueueDepth> remaining_space{QueueDepth};
};

auto main() -> int {
  std::atomic<std::uint64_t> count{0};
  {
    thread_group tg(6, [&](std::stop_token s) {
      while (!s.stop_requested()) {
        ++count;
      }
    });
  }

  std::printf("%ld\n", count.load());

  auto queue = concurrent_bounded_queue<int, 5>{};

  queue.enqueue(5);
  std::cout << queue.dequeue() << '\n';
  std::cout << *queue.try_dequeue() << '\n';

  return EXIT_SUCCESS;
}