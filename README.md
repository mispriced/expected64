# expected64

`expected64` is a tagged union to hold double, int64_t, uint64_t, and (64-bit) pointers.

The core idea is to use a single bit to indicate an `Error` type in a union such as this:

```
union
  {
    T value;
    E error;
  };
```

A user must check if the error bit is set by first calling `has_error()`. Please see the tests for more details but the expected flow is of the nature:

```
    auto result = some_function(some_value);
    if (result.has_error()) // error handing
    else // result.get_value()
```

Each of the four supported types are handled slightly differently.

## Doubles

Doubles use the bits only **after** the `quiet_NaN()` nan mask to store error info.

| Bit | 63  | 62-52       | 51-0                              |
| --- | --- | ----------- | --------------------------------- |
| Use | S   | 11111111111 | Non-zero fraction (not all zeros) |

# Benchmarks

## Catch2 Results

## Benchmark Results

- [GCC 11 Results](https://mispriced.github.io/expected64/benchmark-results-gcc-11/benchmark_results_gcc-11.html)
- [GCC 12 Results](https://mispriced.github.io/expected64/benchmark-results-gcc-12/benchmark_results_gcc-12.html)
- [GCC 13 Results](https://mispriced.github.io/expected64/benchmark-results-gcc-13/benchmark_results_gcc-13.html)
- [Clang 16 Results](https://mispriced.github.io/expected64/benchmark-results-clang-16/benchmark_results_clang-16.html)
- [Clang 17 Results](https://mispriced.github.io/expected64/benchmark-results-clang-17/benchmark_results_clang-17.html)

## Nanobench Results (int64_t)

Ran locally on a i7-14700

| ns/op |           op/s | err% | ins/op | cyc/op |   IPC | bra/op | miss% | total | benchmark                  |
| ----: | -------------: | ---: | -----: | -----: | ----: | -----: | ----: | ----: | :------------------------- |
|  2.12 | 471,147,691.07 | 1.6% |  38.00 |   7.19 | 5.287 |  10.25 |  0.0% |  0.03 | `factorial-raw-int`        |
|  2.94 | 340,087,470.50 | 4.0% |  54.00 |  10.05 | 5.373 |  11.25 |  0.0% |  0.04 | `factorial-optional-int`   |
|  2.80 | 356,678,076.01 | 2.1% |  53.00 |   9.46 | 5.600 |  11.25 |  0.0% |  0.04 | `factorial-tlexpected-int` |
|  2.21 | 452,669,048.44 | 1.7% |  40.00 |   7.53 | 5.312 |  11.25 |  0.0% |  0.03 | `factorial-expected64-int` |

# Building and installing

See the [BUILDING](BUILDING.md) document.

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.

# Licensing

MIT License

Copyright (c) [2024] [Josh G]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
