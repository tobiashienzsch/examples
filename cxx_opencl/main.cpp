#include <CL/cl.hpp>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#ifdef _MSC_VER

#pragma optimize("", off)
inline void DoNotOptimizeDependencySink(const void *) {}
#pragma optimize("", on)

/**
 * @brief Useful for writting benchmarks
 */
template<class T>
void DoNotOptimizeAway(const T &datum) {
    DoNotOptimizeDependencySink(&datum);
}

#else

/**
 * @brief Useful for writting benchmarks
 */
template<typename T>
auto DoNotOptimizeAway(const T &datum) -> void {
    // This version of DoNotOptimizeAway tells the compiler that the asm
    // block will read datum from memory, and that in addition it might read
    // or write from any memory location.  If the memory clobber could be
    // separated into input and output that would be preferable.
    asm volatile("" ::"m"(datum)
                 : "memory");
}
#endif

/**
 * @brief Simple RAII timer. Prints the measured time to log info.
 */
template<class Resolution = std::chrono::microseconds>
class ExecutionTimer final {
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;

public:
    ExecutionTimer() = default;
    ~ExecutionTimer() {
        auto const micros = std::chrono::duration_cast<Resolution>(Clock::now() - mStart).count();
        std::cout << "Elapsed: " << micros << "μs\n";
    }

private:
    Clock::time_point mStart = Clock::now();
};

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
        __kernel void ProcessArray(__global float* data, __global float* outData, float gain) {
            const float sample = data[get_global_id(0)] * gain;
            outData[get_global_id(0)] =  sample;
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
    buffer.resize(1 << 10, 0.5f);
    //    std::cout << "Pre Buffer:\n";
    //    std::for_each(std::begin(buffer), std::end(buffer), [](auto const sample) {
    //        std::cout << " " << sample;
    //    });
    //    std::cout << "\n";

    auto kernel = cl::Kernel{program, "ProcessArray", &err};
    auto queue = cl::CommandQueue{context, device};

    {
        auto timer = ExecutionTimer{};

        auto inputBuffer = cl::Buffer{context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(float) * buffer.size(), buffer.data()};
        auto outputBuffer = cl::Buffer{context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(float) * buffer.size(), nullptr};

        kernel.setArg(0, inputBuffer);
        kernel.setArg(1, outputBuffer);
        kernel.setArg(2, 0.75f);

        {
            auto timer2 = ExecutionTimer{};
            queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange{buffer.size()});
            queue.enqueueTask(kernel);
            queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(float) * buffer.size(), buffer.data());
            DoNotOptimizeAway(outputBuffer);
        }
    }
    {
        auto timer3 = ExecutionTimer{};
        for (auto x = 0; x < 100; x++) {
            std::transform(std::begin(buffer), std::end(buffer), std::begin(buffer), [](auto const sample) -> float {
                return sample * 0.75;
            });

            DoNotOptimizeAway(buffer);
            DoNotOptimizeAway(x);
        }
    }

//    std::cout << "Post Buffer:\n";
//    std::for_each(std::begin(buffer), std::end(buffer), [](auto const sample) {
//        std::cout << " " << sample;
//    });
//    std::cout << "\n";

    return EXIT_SUCCESS;
}
