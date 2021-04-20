#pragma once

#define FOO int

namespace etl
{
// Normal comment
template<typename FloatType>
inline constexpr auto pi = static_cast<FloatType>(3.14159);

/// Documentation comment
template<typename FloatType>
[[nodiscard]] inline auto do_it(FloatType x) -> decltype(auto)
{
    return x * 2;
}

enum class foo
{
    bar,
    baz
};

[[nodiscard]] constexpr auto do_foo(FOO x) -> FOO { return x; }

}  // namespace etl