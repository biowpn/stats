# stats

Basic Statistics for C++.

This project is a partial implementation of the paper:
- [P1708R7 - Basic Statistics](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p1708r7.pdf)

Features implemented:
- Mean (arithmetic mean) (weighted/unweighted)
- Geometric mean (weighted/unweighted)
- Harmonic mean (weighted/unweighted)
- Variance (weighted/unweighted, population/sample)
- Standard Deviation (weighted/unweighted, population/sample)
- Accumulator Objects
- Free Accumulation Functions
- `stats_accumulate`

Features not implemented:
- Skewness (weighted/unweighted, population/sample)
- Kurtosis (weighted/unweighted, population/sample, excess/non-excess)
- Parallel support (execution policies)

Furthermore:
- This implementation adds constructors that accept `stats_data_kind` for `variance` and `variance_accumulator`
    - It is more intuitive and consistent (between the weighted and unweighted cases)


# Example

Use free functions to compute a statistic of an input range:

```cpp
auto values = std::array{1.0, 2.0, 3.0, 4.0, 5.0};
std::cout << xstd::mean(values) << '\n'; // prints 3.0
```

Use accumulators to compute multiple statistics of an input range in one go:

```cpp
// normalize `values`
xstd::mean_accumulator<double> acc_mean;
xstd::standard_deviation_accumulator<double> acc_stdev{xstd::stats_data_kind::population};
xstd::stats_accumulate(values, acc_mean, acc_stdev);
for (auto& x : values) {
    x = (x - acc_mean.value()) / acc_stdev.value();
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
