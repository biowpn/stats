
#include <array>
#include <iostream>

#include <xstd/stats.hpp>

int main()
{
    auto values = std::array{1.0, 2.0, 3.0, 4.0, 5.0};
    std::cout << xstd::mean(values) << '\n';

    xstd::mean_accumulator<double> acc_mean;
    xstd::standard_deviation_accumulator<double> acc_stdev{xstd::stats_data_kind::population};
    xstd::stats_accumulate(values, acc_mean, acc_stdev);
    for (auto& x : values) {
        x = (x - acc_mean.value()) / acc_stdev.value();
    }

    for (auto x : values) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    return 0;
}
