#pragma once

#include "queue.hpp"

#include <algorithm>
#include <atomic>
#include <future>
#include <vector>

namespace mc
{

class ThreadPool
{
public:
    explicit ThreadPool(std::size_t threadCount) : count_ {threadCount}
    {
        for (std::size_t i = 0; i < count_; ++i)
        {
            threads_.emplace_back([&, i] { run(i); });
        }
    }

    ThreadPool(ThreadPool const&) noexcept = delete;
    ThreadPool(ThreadPool&&) noexcept      = delete;

    auto operator=(ThreadPool const&) noexcept -> ThreadPool& = delete;
    auto operator=(ThreadPool&&) noexcept -> ThreadPool& = delete;

    ~ThreadPool() noexcept
    {
        std::for_each(queues_.begin(), queues_.end(), [](auto& q) { q.Done(); });
        std::for_each(threads_.begin(), threads_.end(), [](auto& t) { t.join(); });
    }

    [[nodiscard]] static auto GlobalInstance() -> ThreadPool&;

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

template<typename Function, typename... Args>
auto Async(Function&& f, Args&&... args)
{
    using result_type   = std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;
    using packaged_type = std::packaged_task<result_type()>;

    auto func    = [_f = std::forward<Function>(f)](Args&... ags) { return _f(std::move(ags)...); };
    auto package = new packaged_type([=]() { return func(std::forward<Args>(args)...); });
    auto result  = package->get_future();

    ThreadPool::GlobalInstance().Async([package] {
        (*package)();
        delete package;
    });

    return result;
}

}  // namespace mc
