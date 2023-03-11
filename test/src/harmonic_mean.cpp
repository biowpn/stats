
#include <array>

#include <xstd/stats.hpp>

#include "assert_equal.hpp"

using namespace xstd;

void test_harmonic_mean()
{
    std::array x{40.0, 60.0};
    assert_almost_equal(harmonic_mean(x), 48.0);
}

void test_weighted_harmonic_mean()
{
    std::array x{40.0, 60.0};
    std::array w{5.0, 30.0};
    assert_almost_equal(harmonic_mean(x, w), 56.0);
}

auto main() -> int
{
    test_harmonic_mean();
    test_weighted_harmonic_mean();

    return 0;
}
