#include <CL/cl.hpp>

#include <algorithm>
#include <iostream>
#include <vector>

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
        __kernel void ProcessArray(__global float* data, __global float* outData) {
            outData[get_global_id(0)] = data[get_global_id(0)] * 0.25;
        }
    )"};

    auto sources = cl::Program::Sources{1, std::make_pair(source.c_str(), source.length())};
    auto context = cl::Context{device};
    auto program = cl::Program{context, sources};

    auto err = program.build("-cl-std=CL1.2");
    if (err != CL_SUCCESS) {
        std::cerr << "Error: Building Program Code: " << err << "\n";
    }

    auto buffer = std::vector<float>{};
    buffer.resize(128, 0.5f);
    std::cout << "Pre Buffer:\n";
    std::for_each(std::begin(buffer), std::end(buffer), [](auto const sample) {
      std::cout << " " << sample;
    });
    std::cout << "\n";

    auto inputBuffer = cl::Buffer{context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(float) * buffer.size(), buffer.data()};
    auto outputBuffer = cl::Buffer{context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * buffer.size(), nullptr};

    auto kernel = cl::Kernel{program, "ProcessArray", &err};
    kernel.setArg(0, inputBuffer);
    kernel.setArg(1, outputBuffer);

    auto queue = cl::CommandQueue{context, device};
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange{buffer.size()});
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(float) * buffer.size(), buffer.data());

    std::cout << "Post Buffer:\n";
    std::for_each(std::begin(buffer), std::end(buffer), [](auto const sample) {
        std::cout << " " << sample;
    });
    std::cout << "\n";

    return EXIT_SUCCESS;
}
