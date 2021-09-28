#include <cstdio>
#include <exception>
#include <utility>

namespace mc
{

template<typename Callback>
class ScopeGuard
{
public:
    explicit ScopeGuard(Callback&& callback)
        : callback_ {std::forward<Callback>(callback)}
    {
    }

    ~ScopeGuard() { callback_(); }

private:
    Callback callback_;
};

namespace detail
{

enum class ScopeGuardOnExit
{
};

template<typename Func>
auto operator+(ScopeGuardOnExit /*unused*/, Func&& func) -> ScopeGuard<Func>
{
    return ScopeGuard<Func>(std::forward<Func>(func));
}

struct UncaughtExceptionCounter
{
    UncaughtExceptionCounter() = default;

    [[nodiscard]] auto newUncaughtException() const noexcept -> bool
    {
        return std::uncaught_exceptions() > exceptionCount_;
    }

private:
    int exceptionCount_ {std::uncaught_exceptions()};
};

template<typename FunctionT, bool ExecuteOnException>
struct ScopeGuardForNewException
{
    explicit ScopeGuardForNewException(FunctionT const& fn) : func_ {fn} { }
    explicit ScopeGuardForNewException(FunctionT&& fn) : func_ {std::move(fn)}
    {
    }

    ~ScopeGuardForNewException() noexcept(ExecuteOnException)
    {
        if (ExecuteOnException == ec_.newUncaughtException())
        {
            func_();
        }
    }

private:
    FunctionT func_;
    UncaughtExceptionCounter ec_ {};
};

enum struct ScopeGuardOnFail
{
};

template<typename FuncT>
auto operator+(ScopeGuardOnFail, FuncT&& fn)
    -> ScopeGuardForNewException<std::decay_t<FuncT>, true>
{
    return ScopeGuardForNewException<std::decay_t<FuncT>, true>(
        std::forward<FuncT>(fn));
}

}  // namespace detail

}  // namespace mc

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(name) CONCATENATE(name, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(name) CONCATENATE(name, __LINE__)
#endif

#define SCOPE_EXIT                                                             \
    auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE)                                  \
        = ::mc::detail::ScopeGuardOnExit {} + [&]()  // NOLINT

#define SCOPE_FAIL                                                             \
    auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE)                                  \
        = ::mc::detail::ScopeGuardOnFail {} + [&]() noexcept  // NOLINT

auto foo() -> void
{
    SCOPE_EXIT { std::puts("1"); };
    SCOPE_EXIT { std::puts("2"); };
    SCOPE_EXIT { std::puts("3"); };
    SCOPE_EXIT { std::puts("4"); };

    SCOPE_FAIL { std::puts("fail"); };
    throw 1;
}

auto main() -> int
{
    foo();
    return 0;
}