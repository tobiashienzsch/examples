#include <dispatch/dispatch.h>

#include <cstdio>

#include <thread>

int main()
{
    // auto queue = dispatch_get_main_queue();
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    if (!queue)
    {
        printf("Error: cannot get global dispatch queue\n");
        return 1;
    }

    dispatch_async_f(queue, nullptr, [](void*) { printf("WORK\n"); });
    // dispatch_main();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}