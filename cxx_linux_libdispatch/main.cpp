#include "async.hpp"

#include <cstdio>
#include <cstdlib>

#include <vector>

int main()
{
    auto task = [](int i) -> int {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::printf("%d\n", i);
        return i;
    };

    auto futures = std::vector<std::future<int>> {};

    for (auto i = 0; i < 1000; ++i)
    {
        futures.push_back(mc::Async(mc::AsyncQueue::Default, task, i));
    }

    for (auto& f : futures)
    {
        f.get();
    }

    return 0;
}