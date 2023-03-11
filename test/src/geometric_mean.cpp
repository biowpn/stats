
#include <array>

#include <xstd/stats.hpp>

#include "assert_equal.hpp"

using namespace xstd;

void test_geometric_mean()
{
    std::array x{1.0, 2.0, 4.0};
    assert_equal(geometric_mean(x), 2.0);
}

void test_weighted_geometric_mean()
{
    std::array x{1.0, 2.0, 4.0};
    std::array w{2.0, 1.0, 2.0};
    assert_equal(geometric_mean(x, w), 2.0);
}

auto main() -> int
{
    test_geometric_mean();
    test_weighted_geometric_mean();

    return 0;
}
