#pragma once

#include <cstdlib>

#define RT_ASSERT(expr)                                                                 \
    if (!(expr))                                                                        \
    {                                                                                   \
        RT_ERROR("Assertion failed: {} in file {} line {}", #expr, __FILE__, __LINE__); \
        std::exit(1);                                                                   \
    }

#define RT_ASSERT_MSG(expr, msg)                                                                  \
    if (!(expr))                                                                                  \
    {                                                                                             \
        RT_ERROR("Assertion failed: {} ({}) in file {} line {}", #expr, msg, __FILE__, __LINE__); \
        std::exit(1);                                                                             \
    }
