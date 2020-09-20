#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <ranges>
#include <span>
#include <vector>

namespace rng = std::ranges;

template<typename ValueT, size_t Extent = std::dynamic_extent>
using Span = std::span<ValueT, Extent>;

template<typename ValueT, size_t Extent = std::dynamic_extent>
using BufferView = Span<ValueT, Extent>;
template<typename ValueT, size_t Extent = std::dynamic_extent>
using AudioBufferView = Span<BufferView<ValueT, Extent>>;

class AudioBuffer
{
public:
    AudioBuffer(float* dataToReferTo, int numSamples, int numChannels)
        : data_ {dataToReferTo}, numSamples_ {numSamples}, numChannels_ {numChannels}
    {
    }

    float* data(int channel) { return data_ + numSamples_ * channel; }
    int numSamples() const { return numSamples_; }
    int getNumChannels() const { return numChannels_; }

private:
    float* data_     = nullptr;
    int numSamples_  = 0;
    int numChannels_ = 0;
};

auto ApplyGain(auto audio, float gain) noexcept
{
    rng::for_each(audio, [gain](auto buffer) {
        rng::transform(buffer, std::begin(buffer), [gain](auto sample) noexcept { return sample * gain; });
    });
}

// auto ApplyGainRamp(auto audio, BufferView gain) {
//   rng::for_each(audio, [gain](auto buffer) {
//     rng::transform(
//         buffer, gain, std::begin(buffer),
//         [](auto sample, auto gainValue) { return sample * gainValue; });
//   });
// }

static auto ProcessSubBlock(auto view) noexcept
{
    ApplyGain(view, 0.5f);
    ApplyGain(view, 0.95f);
    ApplyGain(view, 0.25f);
}

auto ProcessBlock(AudioBuffer& buffer) noexcept
{
    constexpr auto SubBufferSize = 32;
    auto const numSamples        = buffer.numSamples();

    // Buffer length is not divisible by SubBufferSize
    auto const remainder = numSamples % SubBufferSize;
    if (remainder > 0)
    {
        auto buffers = std::array {
            BufferView<float> {buffer.data(0), (size_t)remainder},
            BufferView<float> {buffer.data(1), (size_t)remainder},
        };
        auto view = AudioBufferView<float> {buffers};
        ProcessSubBlock(view);

        // Case: AudioBuffer is smaller then fixed size
        if (numSamples < SubBufferSize)
        {
            return;
        }
    }

    // Loop over all subblocks
    auto offset = remainder;
    while (offset + SubBufferSize <= numSamples)
    {
        auto buffers = std::array {
            BufferView<float, SubBufferSize> {buffer.data(0) + offset, SubBufferSize},
            BufferView<float, SubBufferSize> {buffer.data(1) + offset, SubBufferSize},
        };
        auto view = AudioBufferView<float, SubBufferSize> {buffers};
        ProcessSubBlock(view);

        offset += SubBufferSize;
    }
}

int main()
{
    constexpr auto numChannels = 2;
    constexpr auto numSamples  = 4096 * 2048;

    std::vector<float> samples {};
    samples.resize(numSamples * numChannels);
    std::generate(std::begin(samples), std::end(samples), [] {
        static float sample = 0.001f;
        return sample += 0.005f;
    });

    // std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<float>(std::cout, "\n"));

    AudioBuffer buffer {samples.data(), numSamples, numChannels};
    ProcessBlock(buffer);

    // std::copy(std::begin(samples), std::end(samples), std::ostream_iterator<float>(std::cout, "\n"));

    return static_cast<int>(samples[0]);
}