#include <array>
#include <cassert>
#include <cstdlib>

namespace mc
{

struct AllocBlock
{
    void* ptr {nullptr};
    std::size_t size {0};
};

template<typename Primary, typename Fallback>
class FallbackAllocator
    : private Primary
    , private Fallback
{
public:
    [[nodiscard]] auto allocate(std::size_t numBytes) -> AllocBlock
    {
        if (auto const block = Primary::allocate(numBytes);
            block.ptr != nullptr)
        {
            return block;
        }
        return Fallback::allocate(numBytes);
    }

    auto deallocate(AllocBlock block) -> void
    {
        if (Primary::owns(block))
        {
            return Primary::deallocate(block);
        }
        return Fallback::deallocate(block);
    }

    [[nodiscard]] auto owns(AllocBlock block) -> bool
    {
        return Primary::owns(block) || Fallback::owns(block);
    }
};

class NullAllocator
{
public:
    [[nodiscard]] static auto allocate(std::size_t /*numBytes*/) noexcept
        -> AllocBlock
    {
        return AllocBlock {nullptr, 0};
    }

    static auto deallocate(AllocBlock block) noexcept -> void
    {
        assert(block.ptr == nullptr);
    }

    [[nodiscard]] static auto owns(AllocBlock block) noexcept -> bool
    {
        return block.ptr == nullptr;
    }
};

class MallocAllocator
{
public:
    [[nodiscard]] static auto allocate(std::size_t numBytes) -> AllocBlock
    {
        return AllocBlock {std::malloc(numBytes), numBytes};
    }

    static auto deallocate(AllocBlock block) -> void { std::free(block.ptr); }
};

template<std::size_t Size>
class StackAllocator
{
public:
    [[nodiscard]] auto allocate(std::size_t numBytes) noexcept -> AllocBlock
    {
        auto const n = roundToAligned(numBytes);
        if (n > static_cast<size_t>(memory_.data() + Size - next_))
        {
            return AllocBlock {nullptr, 0};
        }
        auto block = AllocBlock {next_, numBytes};
        next_ += n;
        return block;
    }

    auto deallocate(AllocBlock block) noexcept -> void
    {
        if (static_cast<char*>(block.ptr) + roundToAligned(block.size) == next_)
        {
            next_ = static_cast<char*>(block.ptr);
        }
    }

    [[nodiscard]] auto owns(AllocBlock block) const noexcept -> bool
    {
        return block.ptr >= memory_.data() && block.ptr < memory_.data() + Size;
    }

    auto deallocateAll() noexcept -> void { next_ = memory_.data(); }

private:
    [[nodiscard]] static auto roundToAligned(std::size_t n) noexcept
        -> std::size_t
    {
        return n;
    }

    std::array<char, Size> memory_ {};
    char* next_ {memory_.data()};
};

using LocalAllocator = FallbackAllocator<StackAllocator<1024>, NullAllocator>;
}  // namespace mc

auto alloc(mc::LocalAllocator& alloc) -> mc::AllocBlock
{
    return alloc.allocate(128);
}

auto owns(mc::LocalAllocator& alloc, void* ptr, std::size_t n) -> bool
{
    return alloc.owns({ptr, n});
}

auto main() -> int { return 0; }