from time import time

import numba
from numba import vectorize, float64, prange, njit
import numpy as np


def profile_microseconds(func):
    start = time()
    func()
    end = time()
    return (end - start)*1000*1000


def apply_gain_numpy(buffer, gain):
    buffer * gain


@njit(cache=True, nogil=True)
def apply_gain_jit(buffer, gain):
    for sample in buffer:
        sample = sample * gain


def apply_gain_buffer_numpy(buffer, gain):
    buffer * gain


@njit(cache=True, nogil=True)
def apply_gain_buffer_jit(buffer, gain):
    for i in prange(buffer.shape[0]):
        buffer[i] = buffer[i] * gain[i]


def perf_test(name, func):
    print(name)
    results = []
    for _ in range(1024*4):
        results.append(profile_microseconds(func))

    total = int(sum(results))
    minimum = min(results)
    maximum = int(max(results))
    average = int(sum(results)/len(results))

    return (total, minimum, maximum, average)


def main():
    x = np.random.uniform(-10, 10, size=1024*1024).astype(np.float32)
    print(perf_test('numpy', lambda: apply_gain_numpy(x, 0.999)))
    print(perf_test('jit', lambda: apply_gain_jit(x, 0.999)))


if __name__ == "__main__":
    main()
