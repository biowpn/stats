
#include <array>
#include <iostream>

#include <xstd/statistics.hpp>

int main()
{
    // compute the mean of values
    auto values = std::array{1.0, 2.0, 3.0, 4.0, 5.0};
    std::cout << xstd::mean(values) << '\n';

    // normalize `values`
    xstd::mean_accumulator<double> acc_mean;
    xstd::standard_deviation_accumulator<double> acc_stdev{0};  // population: ddof = 0
    xstd::stats_accumulate(values, acc_mean, acc_stdev);
    auto mean = acc_mean.value();
    auto stdev = acc_stdev.value();
    for (auto& x : values) {
        x = (x - mean) / stdev;
    }

    for (auto x : values) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    return 0;
}
