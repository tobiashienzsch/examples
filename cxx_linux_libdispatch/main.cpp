#include "async.hpp"

#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main()
{
    std::vector<std::future<std::vector<char>>> tasks;
    tasks.reserve(128);
    std::generate_n(std::back_inserter(tasks), 128, [] {
        return mc::Async(mc::QueuePriority::Default, [] {
            auto input = std::ifstream("/home/tobante/bin/bin/_pcbnew.kiface", std::ios::binary);
            auto bytes = std::vector<char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
            input.close();
            return bytes;
        });
    });

    std::for_each(begin(tasks), end(tasks), [](auto& f) { std::cout << f.get().size() << '\n'; });

    return 0;
}