#include <CL/sycl.hpp>
#include <cmath>
#include <iostream>

int main(int argc, char* argv[])
{
    sycl::queue queue;

    std::cout << "Using " << queue.get_device().get_info<sycl::info::device::name>()
              << std::endl;

    // Compute the first n_items values in a well known sequence
    constexpr auto n_items = 16;
    auto* items            = sycl::malloc_shared<int>(n_items, queue);
    auto kernel            = [items](sycl::id<1> i) {
        double x1 = pow((1.0 + sqrt(5.0)) / 2, i);
        double x2 = pow((1.0 - sqrt(5.0)) / 2, i);
        items[i]  = round((x1 - x2) / sqrt(5));
    };

    queue.parallel_for(sycl::range<1>(n_items), kernel).wait();
    for (int i = 0; i < n_items; ++i) { std::cout << items[i] << std::endl; }
    free(items, queue);

    return 0;
}