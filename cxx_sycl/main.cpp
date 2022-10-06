#if __has_include(<SYCL/sycl.hpp>)
#include <SYCL/sycl.hpp>
#else
#include <CL/sycl.hpp>
#endif

class hello_world;

auto main() -> int
{
    auto queue = sycl::queue {};

    queue
        .submit(
            [&](sycl::handler& cgh)
            {
                auto os = sycl::stream {128, 128, cgh};
                cgh.single_task<hello_world>([=]() { os << "Hello World!\n"; });
            })
        .wait();
    return 0;
}