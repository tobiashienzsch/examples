#include <cstdio>
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
        = ::mc::detail::ScopeGuardOnExit {} + [&]()

auto main() -> int
{
    SCOPE_EXIT { std::puts("1"); };
    SCOPE_EXIT { std::puts("2"); };
    SCOPE_EXIT { std::puts("3"); };
    SCOPE_EXIT { std::puts("4"); };
    return 0;
}