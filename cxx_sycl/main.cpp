#include <cstdio>
#include <cstdlib>

#if __has_include(<SYCL/sycl.hpp>)
#include <SYCL/sycl.hpp>
#else
#include <CL/sycl.hpp>
#endif

auto toString(sycl::info::device_type type) -> std::string
{
    switch (type)
    {
        case sycl::info::device_type::cpu: return "CPU";
        case sycl::info::device_type::gpu: return "GPU";
        case sycl::info::device_type::accelerator: return "Accelerator";
        case sycl::info::device_type::custom: return "Custom";
        case sycl::info::device_type::automatic: return "Automatic";
        case sycl::info::device_type::host: return "Host";
        case sycl::info::device_type::all: return "All";
    }

    return "Unkown";
}

auto printDeviceInfo(sycl::device dev) -> void
{
    auto name         = dev.get_info<sycl::info::device::name>();
    auto vendor       = dev.get_info<sycl::info::device::vendor>();
    auto type         = dev.get_info<sycl::info::device::device_type>();
    auto maxAllocSize = dev.get_info<sycl::info::device::max_mem_alloc_size>();

    std::printf("----------------------------------------\n");
    std::printf("Name: %s\n", name.c_str());
    std::printf("Vendor: %s\n", vendor.c_str());
    std::printf("Type: %s\n", toString(type).c_str());
    std::printf("Max alloc size: %zu MB\n", maxAllocSize / 1024 / 1024);
    for (auto groupSize : dev.get_info<sycl::info::device::sub_group_sizes>())
    {
        std::printf("Subgroup size: %zu\n", groupSize);
    }
    std::printf("\n");
}

struct UsmDeviceSelector : sycl::device_selector
{
    auto operator()(sycl::device const& dev) const -> int
    {
        if (dev.has(sycl::aspect::usm_device_allocations))
        {
            if (dev.has(sycl::aspect::gpu)) return 2;
            return 1;
        }
        return -1;
    }
};

class hello_world;

auto main() -> int
{
    for (auto dev : cl::sycl::device::get_devices())
    {
        printDeviceInfo(dev);
    }

    auto queue  = sycl::queue {UsmDeviceSelector {}};
    auto device = queue.get_device();
    printDeviceInfo(device);

    try
    {
        queue
            .submit(
                [&](sycl::handler& cgh)
                {
                    auto os = sycl::stream {128, 128, cgh};
                    cgh.single_task<hello_world>([=]()
                                                 { os << "Hello World!\n"; });
                })
            .wait();
    }
    catch (std::exception const& e)
    {
        std::fprintf(stderr, "Exception caught: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}