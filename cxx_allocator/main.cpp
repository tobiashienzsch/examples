#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <array>

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
    [[nodiscard]] auto allocate(std::size_t size) -> AllocBlock
    {
        if (auto const block = Primary::allocate(size); block.ptr != nullptr)
        {
            return block;
        }
        return Fallback::allocate(size);
    }

    auto deallocate(AllocBlock block) -> void
    {
        if (Primary::owns(block))
        {
            Primary::deallocate(block);
            return;
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
    [[nodiscard]] static auto allocate(std::size_t /*size*/) noexcept
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
    [[nodiscard]] static auto allocate(std::size_t size) -> AllocBlock
    {
        return AllocBlock {std::malloc(size), size};
    }

    static auto deallocate(AllocBlock block) -> void { std::free(block.ptr); }

    [[nodiscard]] static auto owns(AllocBlock /*block*/) -> bool
    {
        return true;
    }
};

template<std::size_t Size>
class StackAllocator
{
public:
    [[nodiscard]] auto allocate(std::size_t size) noexcept -> AllocBlock
    {
        auto const n = roundToAligned(size);
        if (n > static_cast<size_t>(memory_.data() + Size - next_))
        {
            return AllocBlock {nullptr, 0};
        }
        auto block = AllocBlock {next_, size};
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

template<typename Upstream>
class LoggingAllocator : private Upstream
{
public:
    [[nodiscard]] auto allocate(std::size_t size) noexcept -> AllocBlock
    {
        auto const block = Upstream::allocate(size);
        std::printf("%-10lu new %p\n", block.size, block.ptr);
        return block;
    }

    auto deallocate(AllocBlock block) noexcept -> void
    {
        std::printf("%-10lu del %p\n", block.size, block.ptr);
        Upstream::deallocate(block);
    }

    [[nodiscard]] auto owns(AllocBlock block) noexcept -> bool
    {
        return Upstream::owns(block);
    }
};

using LocalAllocator = LoggingAllocator<
    FallbackAllocator<StackAllocator<1024>, MallocAllocator>>;
}  // namespace mc

auto main() -> int
{
    auto localAlloc = mc::LocalAllocator {};
    localAlloc.deallocate(localAlloc.allocate(128));
    localAlloc.deallocate(localAlloc.allocate(512));
    localAlloc.deallocate(localAlloc.allocate(1024));
    localAlloc.deallocate(localAlloc.allocate(2048));
    localAlloc.deallocate(localAlloc.allocate(1U << 14));
    localAlloc.deallocate(localAlloc.allocate(1U << 14));
    localAlloc.deallocate(localAlloc.allocate(1U << 16));
    localAlloc.deallocate(localAlloc.allocate(1U << 16));
    localAlloc.deallocate(localAlloc.allocate(1U << 24));

    auto const b1 = localAlloc.allocate(4096);
    auto const b2 = localAlloc.allocate(4096 * 3);
    auto const b3 = localAlloc.allocate(4096 * 6);

    localAlloc.deallocate(b1);
    localAlloc.deallocate(b2);
    localAlloc.deallocate(b3);

    return 0;
}