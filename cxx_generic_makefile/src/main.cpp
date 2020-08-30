#include <cstdlib>
#include <memory>

template<typename... T>
auto ignore_unused(T&&...) -> void
{
}

struct S
{
    int x;
    int y;

    S(int i, int j) : x {i}, y {j} { }
};

int main()
{
    auto s = std::make_unique<S>(1, 2);
    ignore_unused(s);
    return EXIT_SUCCESS;
}
