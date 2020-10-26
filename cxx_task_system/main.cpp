#include <cstdlib>

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
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

class TaskSystem
{
public:
    TaskSystem(std::size_t threadCount = 2) : count_ {threadCount}
    {
        for (std::size_t i = 0; i < count_; ++i)
        {
            threads_.emplace_back([&, i] { run(i); });
        }
    }

    ~TaskSystem() noexcept
    {
        queue_.Done();
        // std::for_each(queue_.begin(), queue_.end(), [](auto& q) { q.done(); });
        std::for_each(threads_.begin(), threads_.end(), [](auto& t) { t.join(); });
    }

    template<typename Func>
    auto Async(Func&& func)
    {
        queue_.Push(std::forward<Func>(func));
    }

private:
    void run(std::size_t /*id*/)
    {
        while (true)
        {
            NotificationQueue::value_type func;
            if (!queue_.Pop(func))
            {
                break;
            }

            func();
        }
    }

    std::size_t count_;
    std::vector<std::thread> threads_;
    NotificationQueue queue_;
};
}  // namespace mc

int main()
{
    mc::TaskSystem dispatcher {8};
    dispatcher.Async([]() { std::cout << std::this_thread::get_id() << '\n'; });
    dispatcher.Async([]() { std::cout << std::this_thread::get_id() << '\n'; });
    dispatcher.Async([]() { std::cout << std::this_thread::get_id() << '\n'; });
    dispatcher.Async([]() { std::cout << std::this_thread::get_id() << '\n'; });
    dispatcher.Async([]() { std::cout << std::this_thread::get_id() << '\n'; });
    dispatcher.Async([]() { std::cout << std::this_thread::get_id() << '\n'; });
    dispatcher.Async([]() { std::cout << std::this_thread::get_id() << '\n'; });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return EXIT_SUCCESS;
}