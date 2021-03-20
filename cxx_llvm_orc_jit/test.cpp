
template<typename T>
struct Data
{
public:
    constexpr explicit Data(T x) : x_ {x} { }

    [[nodiscard]] constexpr auto getValue() const noexcept -> T { return x_; }

private:
    T x_;
};

extern "C" int foo(int x)
{
    auto d = Data(x);
    return d.getValue() + 1;
}

extern "C" void apply_gain(double gain, double* buffer, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        buffer[i] = buffer[i] * gain;
    }
}