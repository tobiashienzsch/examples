#include "wave.hpp"

#include <array>
#include <bit>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

auto toString(WavFormat format) -> std::string
{
    if (format == WavFormat::pcm)
    {
        return "pcm";
    }
    if (format == WavFormat::adpcm)
    {
        return "adpcm";
    }
    if (format == WavFormat::ieee_float)
    {
        return "ieee_float";
    }
    if (format == WavFormat::alaw)
    {
        return "alaw";
    }
    if (format == WavFormat::mulaw)
    {
        return "mulaw";
    }
    if (format == WavFormat::dvi_adpcm)
    {
        return "dvi_adpcm";
    }
    if (format == WavFormat::extensible)
    {
        return "extensible";
    }

    return "";
}

static constexpr auto junkID = std::array<std::byte, 4> {
    std::byte {'J'},
    std::byte {'U'},
    std::byte {'N'},
    std::byte {'K'},
};

template<typename T>
[[nodiscard]] auto fromLittleEndian(std::span<std::byte const, sizeof(T)> src)
    -> T
{
    auto bytes = std::array<std::byte, sizeof(T)> {};
    std::copy(begin(src), end(src), std::begin(bytes));

    if constexpr (std::endian::native == std::endian::big)
    {
        std::reverse(bytes.begin(), bytes.end());
        return std::bit_cast<T>(bytes);
    }

    return std::bit_cast<T>(bytes);
}

struct SpanInputStream
{
    std::span<std::byte const> buf;
};

auto skip(SpanInputStream& s, std::size_t count) -> bool
{
    if (count > size(s.buf))
    {
        return false;
    }
    s.buf = s.buf.subspan(count);
    return true;
}

auto read(SpanInputStream& s, std::span<std::byte> out) -> bool
{
    if (size(out) > size(s.buf))
    {
        return false;
    }

    std::copy(begin(s.buf), std::next(begin(s.buf), ssize(out)), begin(out));
    s.buf = s.buf.subspan(size(out));
    return true;
}

template<size_t Size>
auto readBytesToArray(SpanInputStream& s) -> std::array<std::byte, Size>
{
    auto buffer = std::array<std::byte, Size> {};
    read(s, buffer);
    return buffer;
}

template<typename T>
auto readLittleEndian(SpanInputStream& s) -> T
{
    auto const buffer = readBytesToArray<sizeof(T)>(s);
    return fromLittleEndian<T>(buffer);
}

auto parseWavFileData(std::span<std::byte const> data) -> WavFile
{
    auto s   = SpanInputStream {data};
    auto wav = WavFile {};

    wav.header.magicHeader = readBytesToArray<4>(s);
    wav.header.file_size   = readLittleEndian<uint32_t>(s);
    wav.header.format      = readBytesToArray<4>(s);
    wav.header.subChunkID  = readBytesToArray<4>(s);

    if (wav.header.subChunkID == junkID)
    {
        skip(s, readLittleEndian<uint32_t>(s));
        wav.header.subChunkID = readBytesToArray<4>(s);
    }

    wav.header.subChunkSize  = readLittleEndian<uint32_t>(s);
    wav.header.audioFormat   = readLittleEndian<WavFormat>(s);
    wav.header.channels      = readLittleEndian<uint16_t>(s);
    wav.header.sampleRate    = readLittleEndian<uint32_t>(s);
    wav.header.byteRate      = readLittleEndian<uint32_t>(s);
    wav.header.blockAlign    = readLittleEndian<uint16_t>(s);
    wav.header.bitsPerSample = readLittleEndian<uint16_t>(s);
    wav.header.dataID        = readBytesToArray<4>(s);
    wav.header.dataSize      = readLittleEndian<uint32_t>(s);

    wav.data = s.buf;
    return wav;
}

[[nodiscard]] static auto loadFileAsBytes(char const* path)
{
    auto const file = std::fopen(path, "rb");
    std::fseek(file, 0, SEEK_END);
    auto const fileSize = static_cast<size_t>(std::ftell(file));
    std::fseek(file, 0, SEEK_SET);

    auto buffer = std::vector<std::byte>(fileSize);
    std::fread(data(buffer), 1, size(buffer), file);
    std::fclose(file);
    return buffer;
}

auto main(int, char** argv) -> int
{
    auto const buffer = loadFileAsBytes(argv[1]);
    std::cout << std::format("{}\n", size(buffer));

    auto const wav = parseWavFileData(buffer);
    // std::cout << std::format("ID: {}\n", wav.header.magicHeader);
    std::cout << std::format("File size: {}\n", wav.header.file_size);
    // std::cout << std::format("Format: {}\n", wav.header.format);
    // std::cout << std::format("Chunk ID: {}\n", wav.header.subChunkID);
    std::cout << std::format("Chunk Size: {}\n", wav.header.subChunkSize);
    std::cout << std::format("Channels: {}\n", wav.header.channels);
    std::cout << std::format("Samplerate: {}\n", wav.header.sampleRate);
    std::cout << std::format("Byterate: {}\n", wav.header.byteRate);
    std::cout << std::format("Bits per Sample: {}\n", wav.header.bitsPerSample);
    std::cout << std::format("Block align: {}\n", wav.header.blockAlign);
    std::cout << std::format("Format: {}\n", toString(wav.header.audioFormat));
    // std::cout << std::format("Data ID: {}\n", wav.header.dataID);
    std::cout << std::format("Data Size: {}\n", wav.header.dataSize);

    return 0;
}