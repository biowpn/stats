
#pragma once

#include <cstdlib>
#include <iostream>

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
