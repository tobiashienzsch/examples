# SYCL - Example

## Windows

In normal CMD prompt (not VS Developer Prompt):

```cmd
"%ONEAPI_ROOT%setvars.bat"
cmake -S . -B build -DCMAKE_C_COMPILER=icx -DCMAKE_CXX_COMPILER=icx -GNinja -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

## Resources

- [DPC++ CMake](https://www.intel.com/content/www/us/en/develop/documentation/oneapi-dpcpp-cpp-compiler-dev-guide-and-reference/top/compiler-setup/use-the-command-line/use-cmake-with-the-intel-oneapi-dpc-c-compiler.html)
- [SYCL Workshop Day 1 (19 April 2022)](https://www.youtube.com/watch?v=1hjQGEZftqY)
