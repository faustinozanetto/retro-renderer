#pragma once

#include <filesystem>

#define RT_ASSERT(expr)                                                                                                            \
    if (!(expr))                                                                                                                   \
    {                                                                                                                              \
        RT_ERROR("Assertion failed: {} in file {} line {}", #expr, std::filesystem::path(__FILE__).filename().string(), __LINE__); \
        __debugbreak();                                                                                                            \
    }

#define RT_ASSERT_MSG(expr, msg)                                                                                                             \
    if (!(expr))                                                                                                                             \
    {                                                                                                                                        \
        RT_ERROR("Assertion failed: {} ({}) in file {} line {}", #expr, msg, std::filesystem::path(__FILE__).filename().string(), __LINE__); \
        __debugbreak();                                                                                                                      \
    }
