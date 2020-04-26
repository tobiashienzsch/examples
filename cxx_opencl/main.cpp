#include <iostream>
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

int main() {
    auto all_platforms = std::vector<cl::Platform>{};
    cl::Platform::get(&all_platforms);
    if (all_platforms.size() == 0) {
        std::cout << " No platforms found. Check OpenCL installation!\n";
        return EXIT_FAILURE;
    }

    auto platform = all_platforms.front();
    std::cout << "Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n";

    std::vector<cl::Device> all_devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if (all_devices.size() == 0) {
        std::cout << " No devices found. Check OpenCL installation!\n";
        return EXIT_FAILURE;
    }
    std::cout << "Num devices: " << all_devices.size() << "\n";

    auto default_device = all_devices.front();
    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";
    std::cout << "Using vendor: " << default_device.getInfo<CL_DEVICE_VENDOR>() << "\n";
    std::cout << "Using version: " << default_device.getInfo<CL_DEVICE_VERSION>() << "\n";


    return EXIT_SUCCESS;
}
