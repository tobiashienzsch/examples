#include "pool.hpp"
#include "timer.hpp"

#include <cstdlib>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

int main(int, char**)
{
    std::vector<std::future<std::vector<char>>> tasks;
    tasks.reserve(128);

    {
        mc::ScopedTimer t1 {"Queue"};
        std::generate_n(std::back_inserter(tasks), 128, [] {
            return mc::Async([] {
                auto input = std::ifstream("/home/tobante/bin/bin/_pcbnew.kiface", std::ios::binary);
                auto bytes
                    = std::vector<char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
                input.close();
                return bytes;
            });
        });
    }

    {
        mc::ScopedTimer t2 {"Get"};
        std::for_each(begin(tasks), end(tasks), [](auto& f) { std::cout << f.get().size() << '\n'; });
    }

    return EXIT_SUCCESS;
}