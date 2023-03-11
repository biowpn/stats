
#include <array>
#include <iomanip>

#include <xstd/stats.hpp>

#include "assert_equal.hpp"

using namespace xstd;

void test_variance()
{
    std::array x{1.0, 2.0, 3.0};
    assert_almost_equal(variance(x, xstd::stats_data_kind::population), 0.6666666666666666);
    assert_almost_equal(variance(x, xstd::stats_data_kind::sample), 1.0);
}

void test_weighted_variance()
{
    std::array x{1.0, 2.0, 3.0};
    std::array w{1.0, 1.0, 2.0};
    assert_almost_equal(variance(x, w, xstd::stats_data_kind::population), 0.6875);
    assert_almost_equal(variance(x, w, xstd::stats_data_kind::sample), 1.1000);
}

auto main() -> int
{
    test_variance();
    test_weighted_variance();

    return 0;
}
