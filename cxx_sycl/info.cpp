#include <sycl/sycl.hpp>

#include <cmath>
#include <iostream>

int main(int argc, char* argv[])
{
    auto queue = sycl::queue{};

    std::cout << "Using " << queue.get_device().get_info<sycl::info::device::name>()
              << std::endl;

    return 0;
}