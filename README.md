# stats

Basic Statistics for C++.

This project is a partial implementation of the paper:
- [P1708R8 - Basic Statistics](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p1708r8.pdf)
- A copy of the paper is stored [here](paper/p1708r8.pdf)

Features implemented:
- Mean (arithmetic mean) (weighted/unweighted)
- Geometric mean (weighted/unweighted)
- Harmonic mean (weighted/unweighted)
- Variance (weighted/unweighted, population/sample)
- Standard Deviation (weighted/unweighted, population/sample)
- Accumulator Objects
- Free Accumulation Functions
- `stats_accumulate`

Features **not** implemented:
- Skewness (weighted/unweighted, population/sample)
- Kurtosis (weighted/unweighted, population/sample, excess/non-excess)
- Parallel support (execution policies)



# Example

Use free functions to compute a statistic of an input range:

```cpp
// compute the mean of values
auto values = std::array{1.0, 2.0, 3.0, 4.0, 5.0};
std::cout << xstd::mean(values) << '\n'; // prints 3.0
```

Use accumulators to compute multiple statistics in one go:

```cpp
// normalize `values`
xstd::mean_accumulator<double> acc_mean;
xstd::standard_deviation_accumulator<double> acc_stdev{0};  // population: ddof = 0
xstd::stats_accumulate(values, acc_mean, acc_stdev);
auto mean = acc_mean.value();
auto stdev = acc_stdev.value();
for (auto& x : values) {
    x = (x - mean) / stdev;
}
```

# Build and run tests

```sh
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
cmake --build . -j
ctest -V
```

# Contributing

This project is work-in-progress. You are welcome to provide feedback (via issues) and make changes (via pull requests).

# Licensing

See [LICENSE](LICENSE).
