#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>

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

}  // namespace mc