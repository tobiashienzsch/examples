#include <condition_variable>
#include <future>
#include <iostream>
#include <thread>
#include <variant>

namespace mc
{

namespace detail
{
template<typename PromiseT, typename Cont>
struct ThenPromise
{
    PromiseT p_;
    Cont cont_;

    template<typename... Values>
    auto set_value(Values... vs) -> void
    {
        p_.set_value(cont_(vs...));
    }

    template<typename Exception>
    auto set_exception(Exception e) -> void
    {
        p_.set_exception(e);
    }
};

template<typename T>
struct SyncWaitState
{
    std::mutex Mutex;
    std::condition_variable CV;
    std::variant<std::monostate, std::exception_ptr, T> Data;
};

template<typename T>
struct SyncWaitPromise
{
    SyncWaitState<T>* Pst;

    template<int I, typename... V>
    auto set(V... xs)
    {
        auto lk = std::unique_lock {Pst->Mutex};
        Pst->Data.template emplace<I>(xs...);
        Pst->CV.notify_one();
    }

    template<typename... Values>
    auto set_value(Values... vs) -> void
    {
        set<2>(vs...);
    }

    template<typename Exception>
    auto set_exception(Exception e) -> void
    {
        set<1>(e);
    }
};
}  // namespace detail

struct Sink
{

    template<typename... Values>
    auto set_value(Values... /*vs*/) -> void
    {
    }

    template<typename Exception>
    auto set_exception(Exception /*e*/) -> void
    {
        std::terminate();
    }
};

template<typename LazyFuture, typename Cont>
auto Then(LazyFuture future, Cont cont)
{
    return [=](auto p) { return future(detail::ThenPromise<decltype(p), Cont> {p, cont}); };
}

template<typename T, typename Task>
auto SyncWait(Task task)
{
    // State
    detail::SyncWaitState<T> state;

    // Launch the operation
    task(detail::SyncWaitPromise<T> {&state});

    // Wait
    {
        auto lk = std::unique_lock {state.Mutex};
        state.CV.wait(lk, [&state] { return state.Data.index() != 0; });
    }

    // Throw or Return
    if (state.Data.index() == 1)
    {
        std::rethrow_exception(std::get<1>(state.Data));
    }

    return std::move(std::get<2>(state.Data));
}

auto NewThread()
{
    return [](auto p) {
        std::thread thread {[p = std::move(p)]() mutable { p.set_value(); }};
        thread.detach();
    };
}

template<typename Task>
auto Async(Task task)
{
    return mc::Then(mc::NewThread(), [=] { return task(); });
}
}  // namespace mc

int main()
{
    auto f  = mc::Async([] { return 42; });
    auto f2 = mc::Then(f, [](auto i) { return i + i; });
    std::cout << mc::SyncWait<int>(f2) << '\n';
    return 0;
}