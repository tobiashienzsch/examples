#pragma once

#include "XStopWatch.h"
#include <list>

////////////////////////////////
// Benchmarker
class Benchmarker
{
public:
    Benchmarker() {};
    virtual ~Benchmarker() {};

    // BeginRun
    inline void BeginRun() { m_runTime.Reset(); };

    // EndRun
    inline void EndRun()
    {
        liTimes.push_back((float)m_runTime.GetElapsedTime());
    };

    // Get average execution time for (middle 50% of) all recorded runs
    float GetAverageTime()
    {
        liTimes.sort();
        float middleTimes = 0.f;
        auto a            = liTimes.begin();

        // We sample the middle 50% of times, ignore any outliers from e.g. core
        // bounce, thread interrupt
        for (int k = 0; k < (liTimes.size() / 4); a++, k++)
        {
        };  // Ignore the first 25%
        // float loTime = *a / nRunsPerTimer;           // Get the fastest (25th
        // percentile) time
        for (std::int32_t k = 0; k < (liTimes.size() / 2); a++, k++)
            middleTimes += *a;  // Take the next 50%
        // float hiTime = *a  / nRunsPerTimer;           // Get the slowest
        // (75th percentile) time
        return middleTimes
               / (liTimes.size()
                  / 2.f);  // average execution time for a timer-block
    };

protected:
    StopWatch m_wallTime;
    StopWatch m_runTime;
    std::list<float> liTimes;  // in msec
};
