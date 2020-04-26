#include <CL/cl.hpp>

#include <iostream>

int main(int, char **) {
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

    auto device = all_devices.front();
    std::cout << "Using device: " << device.getInfo<CL_DEVICE_NAME>() << "\n";
    std::cout << "Using vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << "\n";
    std::cout << "Using version: " << device.getInfo<CL_DEVICE_VERSION>() << "\n";

    auto source = std::string{R"(
        __kernel void HelloWorld(__global char* data) {
            data[0] = 'H';
            data[1] = 'e';
            data[2] = 'l';
            data[3] = 'l';
            data[4] = 'o';
            data[5] = ' ';
            data[6] = 'W';
            data[7] = 'o';
            data[8] = 'r';
            data[9] = 'l';
            data[10] = 'd';
        }
    )"};

    auto sources = cl::Program::Sources{1, std::make_pair(source.c_str(), source.length())};
    auto context = cl::Context{device};
    auto program = cl::Program{context, sources};

    auto err = program.build();
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Building Program Code: " << err << "\n";
    }

    char buf[16] = {};
    auto memBuffer = cl::Buffer{context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf)};
    auto kernel = cl::Kernel{program, "HelloWorld", &err};
    kernel.setArg(0, memBuffer);

    auto queue = cl::CommandQueue{context, device};
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(memBuffer, CL_TRUE, 0, sizeof(buf), buf);

    std::cout << "Buffer: " << buf << "\n";

    return EXIT_SUCCESS;
}
