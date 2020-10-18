#include "tuple.hpp"

int main()
{
    struct S
    {
        S() = default;

    private:
        int x, y;
    };
    auto t1 = etl::tuple<S, float> {};
    auto t2 = etl::tuple<char, float, char, float, short, int>();
    auto t3 = etl::tuple<float, float, char, char, short, int>();
    return sizeof(t2);
}