
#include <vector>

#include <xstd/statistics.hpp>

#include "assert_equal.hpp"

using namespace xstd;

void test_mean()
{
    std::vector<double> x{1, 2, 3};
    assert_equal(mean(x), 2.0);
}

void test_weighted_mean()
{
    std::vector<double> x{1, 2, 3};
    std::vector<double> w{1, 1, 2};
    assert_equal(mean(x, w), 2.25);
}

auto main() -> int
{
    test_mean();
    test_weighted_mean();

    return 0;
}
