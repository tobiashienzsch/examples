#include <sys/time.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <type_traits>
#include <vector>

namespace mc
{

class NotificationQueue
{
    using Lock = std::unique_lock<std::mutex>;

public:
    using value_type = std::function<void()>;

    template<typename Func>
    auto Push(Func&& func) -> void
    {
        {
            Lock lock {mutex_};
            queue_.emplace_back(std::forward<Func>(func));
        }

        ready_.notify_one();
    }

    template<typename Func>
    auto TryPush(Func&& func) -> bool
    {
        {
            Lock lock {mutex_};
            if (!lock)
            {
                return false;
            }
            queue_.emplace_back(std::forward<Func>(func));
        }

        ready_.notify_one();
        return true;
    }

    auto Pop(value_type& x) -> bool
    {
        Lock lock {mutex_};
        while (queue_.empty() && !done_)
        {
            ready_.wait(lock);
        }

        if (queue_.empty())
        {
            return false;
        }

        x = std::move(queue_.front());
        queue_.pop_front();
        return true;
    }

    auto TryPop(value_type& x) -> bool
    {
        Lock lock {mutex_, std::try_to_lock};
        if (!lock || queue_.empty())
        {
            return false;
        }

        x = std::move(queue_.front());
        queue_.pop_front();
        return true;
    }

    auto Done() -> void
    {
        {
            Lock lock {mutex_};
            done_ = true;
        }

        ready_.notify_all();
    }

private:
    std::deque<value_type> queue_;
    std::mutex mutex_;
    std::condition_variable ready_;
    bool done_ {false};
};

class ThreadPool
{
public:
    ThreadPool(std::size_t threadCount = 2) : count_ {threadCount}
    {
        for (std::size_t i = 0; i < count_; ++i)
        {
            threads_.emplace_back([&, i] { run(i); });
        }
    }

    ~ThreadPool() noexcept
    {
        std::for_each(queues_.begin(), queues_.end(), [](auto& q) { q.Done(); });
        std::for_each(threads_.begin(), threads_.end(), [](auto& t) { t.join(); });
    }

    static auto Instance() -> ThreadPool&;

    template<typename Func>
    auto Async(Func&& func)
    {
        auto const i     = index_++;
        constexpr auto K = 2;

        for (std::size_t n = 0; n != count_ * K; ++n)
        {
            if (queues_[(i + n) % count_].TryPush(std::forward<Func>(func)))
            {
                return;
            }
        }

        queues_[i % count_].Push(std::forward<Func>(func));
    }

private:
    void run(std::size_t id)
    {
        while (true)
        {
            NotificationQueue::value_type func;

            for (std::size_t n = 0; n != count_; ++n)
            {
                if (auto const index = (id + n) % count_; queues_[index].TryPop(func))
                {
                    break;
                }
            }

            if (!func && !queues_[id].Pop(func))
            {
                break;
            }

            func();
        }
    }

    std::size_t count_;
    std::vector<std::thread> threads_;
    std::vector<NotificationQueue> queues_ {count_};
    std::atomic<std::size_t> index_ {0};
};

static ThreadPool ThreadPoolInstance {8};
auto ThreadPool::Instance() -> ThreadPool& { return ThreadPoolInstance; }

template<typename Function, typename... Args>
auto Async(Function&& f, Args&&... args)
{
    using result_type   = std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;
    using packaged_type = std::packaged_task<result_type()>;

    auto func    = [_f = std::forward<Function>(f)](Args&... args) { return _f(std::move(args)...); };
    auto package = new packaged_type(std::bind(func, std::forward<Args>(args)...));
    auto result  = package->get_future();

    ThreadPool::Instance().Async([package] {
        (*package)();
        delete package;
    });

    return result;
}

class ScopedTimer
{
public:
    explicit ScopedTimer(std::string name) : name_ {std::move(name)} { gettimeofday(&startTime_, nullptr); }

    ~ScopedTimer()
    {
        timeval stopTime;
        gettimeofday(&stopTime, nullptr);
        double const elapsedSec = (stopTime.tv_sec - startTime_.tv_sec) * 1000.0;
        double const elapsedMs  = (stopTime.tv_usec - startTime_.tv_usec) / 1000.0;
        printf("%s: %.4f ms\n", name_.c_str(), elapsedSec + elapsedMs);
    }

private:
    timeval startTime_;
    std::string name_;
};

}  // namespace mc

int main(int, char**)
{
    std::vector<std::future<std::vector<char>>> tasks;
    tasks.reserve(128);

    {
        mc::ScopedTimer t1 {"Queue"};
        std::generate_n(std::back_inserter(tasks), 128, [] {
            return mc::Async([] {
                auto input = std::ifstream("/home/tobante/bin/bin/_pcbnew.kiface", std::ios::binary);
                auto bytes
                    = std::vector<char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
                input.close();
                return bytes;
            });
        });
    }

    {
        mc::ScopedTimer t2 {"Get"};
        std::for_each(begin(tasks), end(tasks), [](auto& f) { std::cout << f.get().size() << '\n'; });
    }

    return EXIT_SUCCESS;
}