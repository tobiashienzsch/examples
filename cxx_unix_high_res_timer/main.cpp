#include <cmath>
#include <cstdio>
#include <sys/time.h>

class HighResolutionTimer {
public:
  explicit HighResolutionTimer() { gettimeofday(&startTime_, nullptr); }

  ~HighResolutionTimer() {
    timeval stopTime;
    gettimeofday(&stopTime, nullptr);

    // compute and print the elapsed time in millisec
    // sec to ms
    double const elapsedSec = (stopTime.tv_sec - startTime_.tv_sec) * 1000.0;
    // us to ms
    double const elapsedMs = (stopTime.tv_usec - startTime_.tv_usec) / 1000.0;

    printf("%ld us | ", (stopTime.tv_usec - startTime_.tv_usec));
    printf("%.2f ms\n", elapsedSec + elapsedMs);
  }

private:
  timeval startTime_;
};

int main() {
  auto returnVal = 0.0;
  {
    auto const timer = HighResolutionTimer{};
    // do something
    for (auto i = 0; i < 100'000; i++) {
      returnVal += std::sin(i);
    }
  }

  return static_cast<int>(returnVal);
}
