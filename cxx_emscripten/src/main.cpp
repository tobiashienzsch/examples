#include <atomic>
#include <iostream>

int main()
{
    std::atomic<float> value {1.0};
    std::cout << "Value: " << value.load() << "\n";
    return 0;
}
