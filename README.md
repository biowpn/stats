# stats

Basic Statistics for C++17 or later.

This project is a partial implementation of the paper:
- [P1708R7 - Basic Statistics](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p1708r7.pdf)

Features implemented:
- Mean (arithmetic mean) (weighted/unweighted)
- Geometric mean (weighted/unweighted)
- Harmonic mean (weighted/unweighted)
- Variance (weighted/unweighted, population/sample)
- Standard Deviation (weighted/unweighted, population/sample)
- Accumulator Objects
- Free-standing Accumulation Functions
- `stats_accumulate`

Features not implemented:
- Skewness (weighted/unweighted, population/sample)
- Kurtosis (weighted/unweighted, population/sample, excess/non-excess)
- Parallel function overloads (execution policies)

Furthermore:
- To support C++17:
    - Concepts and `requires` clauses are dropped
- Add overload for `variance` (and `variance_accumulator` constructor) that accepts `stats_data_kind` as it is more intuitive and consistent


# Example

Use free-standing functions to compute a statistic of an input range:

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

# Contributing

This project is work-in-progress. You are welcome to provide feedback (via issues) and make changes (via pull requests).

# Licensing

See [LICENSE](LICENSE).
