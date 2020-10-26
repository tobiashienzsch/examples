#pragma once

#include <dispatch/dispatch.h>

#include <cassert>

#include <functional>
#include <future>
#include <type_traits>

namespace mc
{

enum class AsyncQueue
{
    Default,
    Low,
    High,
    Message,
};

namespace detail
{

template<typename Function, typename... Args>
auto AsyncImpl(dispatch_queue_t queue, Function&& f, Args&&... args)
{
    using result_type   = std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;
    using packaged_type = std::packaged_task<result_type()>;

    auto func    = [_f = std::forward<Function>(f)](Args&... args) { return _f(std::move(args)...); };
    auto package = new packaged_type(std::bind(func, std::forward<Args>(args)...));
    auto result  = package->get_future();

    dispatch_async_f(
        queue, package, +[](void* userData) {
            auto package = static_cast<packaged_type*>(userData);
            (*package)();
            delete package;
        });

    return result;
}

auto SelectDispatchQueue(AsyncQueue queueType) -> dispatch_queue_t
{
    switch (queueType)
    {
        case AsyncQueue::Default:
        {
            return dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
        }
        case AsyncQueue::Low:
        {
            return dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0);
        }
        case AsyncQueue::High:
        {
            return dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
        }
        case AsyncQueue::Message:
        {
            return dispatch_get_main_queue();
        }
    }
}

}  // namespace detail

template<typename Function, typename... Args>
auto Async(AsyncQueue queueType, Function&& f, Args&&... args)
{
    auto queue = detail::SelectDispatchQueue(queueType);
    assert(queue);
    return detail::AsyncImpl(queue, std::forward<Function>(f), std::forward<Args>(args)...);
}

}  // namespace mc