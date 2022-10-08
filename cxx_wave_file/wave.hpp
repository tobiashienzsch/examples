#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

enum struct WavFormat
{
    pcm        = 0x1,
    adpcm      = 0x2,
    ieee_float = 0x3,
    alaw       = 0x6,
    mulaw      = 0x7,
    dvi_adpcm  = 0x11,
    extensible = 0xFFFE,
};

[[nodiscard]] auto toString(WavFormat format) -> std::string;

struct WavHeader
{
    // Should contain the letters "RIFF"
    std::array<std::byte, 4> magicHeader;

    // File size, minus chunk_id and chunk_size.
    uint32_t file_size;

    // Should contain the letters "WAVE"
    std::array<std::byte, 4> format;

    // Should contain the letters "fmt "
    std::array<std::byte, 4> subChunkID;

    // 16 for PCM.
    uint32_t subChunkSize;

    //
    WavFormat audioFormat;

    // mono = 1, stereo = 2, etc.
    uint16_t channels;

    // self-explanatory
    uint32_t sampleRate;

    // sampleRate * channels * bits per sample / 8
    uint32_t byteRate;

    // Bytes for one sample including all channels.
    uint16_t blockAlign;

    // self-explanatory. BITS, not BYTES.
    uint16_t bitsPerSample;

    // Should contain the letters "data"
    std::array<std::byte, 4> dataID;

    // samples * channels * bits per sample / 8
    uint32_t dataSize;
};

// WAV file with header and pointer to data
struct WavFile
{
    WavHeader header;
    std::span<std::byte const> data;
};

// Parse the header of WAV file and return WavFile structure with header and
// pointer to data
[[nodiscard]] auto parseWavFileData(std::span<std::byte const> data) -> WavFile;
