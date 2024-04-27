
#include <array>
#include <iomanip>

#include <xstd/statistics.hpp>

#include "assert_equal.hpp"

using namespace xstd;

void test_standard_deviation()
{
    std::array x{1.0, 2.0, 3.0};
    assert_almost_equal(standard_deviation(x, 0), 0.81649658);
    assert_almost_equal(standard_deviation(x, 1), 1.0);
}

void test_weighted_standard_deviation()
{
    std::array x{1.0, 2.0, 3.0};
    std::array w{1.0, 1.0, 2.0};
    assert_almost_equal(standard_deviation(x, w, xstd::statistics_data_kind::population), 0.82915620);
    assert_almost_equal(standard_deviation(x, w, xstd::statistics_data_kind::sample), 1.04880885);
}

auto main() -> int
{
    test_standard_deviation();
    test_weighted_standard_deviation();

    return 0;
}
