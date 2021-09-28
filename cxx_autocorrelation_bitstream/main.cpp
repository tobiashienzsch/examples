#define _USE_MATH_DEFINES
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <vector>

// smallest power of 2 that fits n
template<typename T>
constexpr T smallest_pow2(T n, T m = 1)
{
    return (m < n) ? smallest_pow2(n, m << 1) : m;
}

std::uint32_t count_bits(std::uint32_t input)
{
    auto count = std::uint32_t {0};
    while (input)
    {
        count = count + (input & std::uint32_t {1});
        input = input >> std::uint32_t {1};
    }
    return count;
}

std::uint64_t count_bits(std::uint64_t input)
{
    auto count = std::uint64_t {0};
    while (input)
    {
        count = count + (input & std::uint64_t {1});
        input = input >> std::uint64_t {1};
    }
    return count;
}

template<typename T = std::uint32_t>
struct bitstream
{
    static_assert(std::is_unsigned<T>::value, "T must be unsigned");
    static constexpr auto nbits = 8 * sizeof(T);

    bitstream(std::size_t size_)
    {
        size       = smallest_pow2(size_);
        array_size = size / nbits;
        bits.resize(array_size, 0);
    }

    void clear() { std::fill(bits.begin(), bits.end(), 0); }

    void set(std::uint32_t i, bool val)
    {
        auto mask = 1 << (i % nbits);
        auto& ref = bits[i / nbits];
        ref ^= (-T(val) ^ ref) & mask;
    }

    bool get(std::uint32_t i) const
    {
        auto mask = 1 << (i % nbits);
        return (bits[i / nbits] & mask) != 0;
    }

    template<typename F>
    void auto_correlate(std::size_t start_pos, F f)
    {
        auto mid_array = (array_size / 2) - 1;
        auto mid_pos   = size / 2;
        auto index     = start_pos / nbits;
        auto shift     = start_pos % nbits;

        for (auto pos = start_pos; pos != mid_pos; ++pos)
        {
            auto* p1   = bits.data();
            auto* p2   = bits.data() + index;
            auto count = 0;

            if (shift == 0)
            {
                for (unsigned i = 0; i != mid_array; ++i)
                {
                    count += count_bits(*p1++ ^ *p2++);
                }
            }
            else
            {
                auto shift2 = nbits - shift;
                for (unsigned i = 0; i != mid_array; ++i)
                {
                    auto v = *p2++ >> shift;
                    v |= *p2 << shift2;
                    count += count_bits(*p1++ ^ v);
                }
            }
            ++shift;
            if (shift == nbits)
            {
                shift = 0;
                ++index;
            }

            f(pos, count);
        }
    }

    std::vector<T> bits;
    std::size_t size;
    std::size_t array_size;
};

struct zero_cross
{
    bool operator()(float s)
    {
        if (s < -0.1f)
        {
            y = false;
        }
        else if (s > 0.0f)
        {
            y = true;
        }
        return y;
    }

    bool y = false;
};

struct noise
{
    auto operator()() const -> float
    {
        return (float(rand()) / (RAND_MAX / 2)) - 1.0;
    }
};

int main()
{
    constexpr auto pi       = M_PI;
    constexpr auto sps      = 44100;  // 20000;
    constexpr auto min_freq = 50.0;
    constexpr auto max_freq = 500.0;
    constexpr float freq    = 440.0;  // 82.41;
    // constexpr float freq    = 261.626;  // 82.41;

    // These are in samples
    constexpr float period     = float(sps) / freq;
    constexpr float min_period = float(sps) / max_freq;
    constexpr float max_period = float(sps) / min_freq;

    ////////////////////////////////////////////////////////////////////////////
    // Generate a test signal

    constexpr float noise_level = 0.0;  // Noise level (dB)
    constexpr float _1st_level  = 0.3;  // Fundamental level
    constexpr float _2nd_level  = 0.4;  // Second harmonic level
    constexpr float _3rd_level  = 0.3;  // Third harmonic level

    constexpr float offset = 0;
    // constexpr float offset = period - 1.3; // Initial offset (some odd
    // number)
    std::size_t buff_size
        = smallest_pow2<std::size_t>(std::ceil(max_period)) * 2;

    std::vector<float> signal(buff_size);
    noise ns;  // noise

    for (std::size_t i {0}; i < buff_size; i++)
    {
        auto angle = (i + offset) / period;
        signal[i]  = noise_level * ns();                     // Noise
        signal[i] += _1st_level * std::sin(2 * pi * angle);  // First harmonic
        signal[i] += _2nd_level * std::sin(4 * pi * angle);  // Second harmonic
        signal[i] += _3rd_level * std::sin(6 * pi * angle);  // Third harmonic
    }

    ////////////////////////////////////////////////////////////////////////////
    // The bitstream

    bitstream<> bin(buff_size);

    ////////////////////////////////////////////////////////////////////////////
    // Zero crossing
    zero_cross zc;

    for (std::size_t i {0}; i != buff_size; ++i)
    {
        bin.set(i, zc(signal[i]));
    }

    ////////////////////////////////////////////////////////////////////////////
    // Binary Auto-correlation

    std::uint32_t max_count = 0;
    std::uint32_t min_count = UINT32_MAX;
    std::size_t est_index   = 0;
    std::vector<std::uint32_t> corr(buff_size / 2);
    bin.auto_correlate(
        min_period,
        [&corr, &max_count, &min_count, &est_index](auto pos, auto count)
        {
            corr[pos] = count;
            max_count = std::max<std::uint32_t>(max_count, count);
            if (static_cast<unsigned>(count) < min_count)
            {
                min_count = count;
                est_index = pos;
            }
        });

    ////////////////////////////////////////////////////////////////////////////
    // Handle harmonics
    auto sub_threshold = 0.15 * max_count;
    int max_div        = est_index / min_period;
    for (int div = max_div; div != 0; div--)
    {
        bool all_strong = true;
        float mul       = 1.0f / div;

        for (int k = 1; k != div; k++)
        {
            int sub_period = k * est_index * mul;
            if (corr[sub_period] > sub_threshold)
            {
                all_strong = false;
                break;
            }
        }

        if (all_strong)
        {
            est_index = est_index * mul;
            break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Estimate the pitch

    // Get the start edge
    auto prev       = 0.0F;
    auto start_edge = signal.begin();
    for (; *start_edge <= 0.0F; ++start_edge)
    {
        prev = *start_edge;
    }
    auto dy  = *start_edge - prev;
    auto dx1 = -prev / dy;

    // Get the next edge
    auto next_edge = signal.begin() + est_index - 1;
    for (; *next_edge <= 0.0F; ++next_edge)
    {
        prev = *next_edge;
    }
    dy       = *next_edge - prev;
    auto dx2 = -prev / dy;

    float n_samples = (next_edge - start_edge) + (dx2 - dx1);
    float estimate  = sps / n_samples;

    std::cout << "Actual Frequency: " << freq << " Hz" << '\n';
    std::cout << "Estimated Frequency: " << estimate << " Hz" << '\n';
    std::cout << "Error: " << 1200.0 * std::log2(estimate / freq) << " cents"
              << '\n';
    std::cout << "Periodicity: " << 1.0 - (float(min_count) / max_count)
              << '\n';

    return 0;
}