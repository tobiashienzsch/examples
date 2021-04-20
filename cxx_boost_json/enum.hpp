#if not defined(ENUM_HPP)
#define ENUM_HPP

enum Foo
{
    Bar = 1,
    Baz,
};

enum class Position
{
    Up,
    Down,
};

namespace test
{
enum struct State : unsigned char
{
    Idle,
    Active,
    Unknown,
};
}
#endif