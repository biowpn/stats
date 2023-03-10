
#include <vector>

#include <xstd/stats.hpp>

#include "assert_equal.hpp"

using namespace xstd;

void test_geometric_mean()
{
    std::vector<double> x{1, 2, 4};
    assert_equal(geometric_mean(x), 2.0);
}

void test_weighted_geometric_mean()
{
    std::vector<double> x{1, 2, 4};
    std::vector<double> w{2, 1, 2};
    assert_equal(weighted_geometric_mean(x, w), 2.0);
}

auto main() -> int
{
    test_geometric_mean();
    test_weighted_geometric_mean();

    return 0;
}
