
#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

template<class T>
struct approx
{
    T v;
    constexpr approx(T _v) noexcept
        : v(_v)
    {
    }
    bool operator==(approx rhs) const
    {
        return std::abs(v - rhs.v) < 1e-7;
    }
    friend std::ostream& operator<<(std::ostream& os, approx x)
    {
        return os << x.v;
    }
};

#define assert_equal(lhs, rhs) \
    do { \
        if (lhs == rhs) \
            break; \
        std::cerr << __FILE__ << ":" << __LINE__ << ":  assertion failed:\n" \
                  << "    " << #lhs << " == " << #rhs << "\n" \
                  << "    (" << (lhs) << ")  vs  (" << (rhs) << ")" \
                  << "\n"; \
        std::exit(EXIT_FAILURE); \
    } while (0)

#define assert_almost_equal(lhs, rhs) assert_equal(approx(lhs), approx(rhs))
