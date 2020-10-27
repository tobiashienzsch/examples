#pragma once

#include <sys/time.h>

#include <cmath>
#include <cstdio>

#include <string>
#include <utility>

namespace mc
{
class ScopedTimer
{
public:
    explicit ScopedTimer(std::string name) : name_ {std::move(name)} { gettimeofday(&startTime_, nullptr); }

    ScopedTimer(ScopedTimer const&) noexcept = default;
    ScopedTimer(ScopedTimer&&) noexcept      = default;

    auto operator=(ScopedTimer const&) noexcept -> ScopedTimer& = default;
    auto operator=(ScopedTimer&&) noexcept -> ScopedTimer& = default;

    ~ScopedTimer()
    {
        timeval stopTime {};
        gettimeofday(&stopTime, nullptr);
        auto const elapsedSec = static_cast<double>(stopTime.tv_sec - startTime_.tv_sec) * 1000.0;
        auto const elapsedMs  = static_cast<double>(stopTime.tv_usec - startTime_.tv_usec) / 1000.0;
        printf("%s: %.4f ms\n", name_.c_str(), elapsedSec + elapsedMs);
    }

private:
    timeval startTime_ {};
    std::string name_;
};
}  // namespace mc
