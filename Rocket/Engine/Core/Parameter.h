#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>

namespace Rocket {
    // Limits
    constexpr int MAX_INT           = (std::numeric_limits<int>::max)();
    constexpr int MIN_INT           = (std::numeric_limits<int>::min)();
    constexpr int8_t MAX_INT8       = (std::numeric_limits<int8_t>::max)();
    constexpr int8_t MIN_INT8       = (std::numeric_limits<int8_t>::min)();
    constexpr int16_t MAX_INT16     = (std::numeric_limits<int16_t>::max)();
    constexpr int16_t MIN_INT16     = (std::numeric_limits<int16_t>::min)();
    constexpr int32_t MAX_INT32     = (std::numeric_limits<int32_t>::max)();
    constexpr int32_t MIN_INT32     = (std::numeric_limits<int32_t>::min)();
    constexpr int64_t MAX_INT64     = (std::numeric_limits<int64_t>::max)();
    constexpr int64_t MIN_INT64     = (std::numeric_limits<int64_t>::min)();
    constexpr double MAX_DOUBLE     = (std::numeric_limits<double>::max)();
    constexpr double MIN_DOUBLE     = (std::numeric_limits<double>::min)();
    constexpr float MAX_FLOAT       = (std::numeric_limits<float>::max)();
    constexpr float MIN_FLOAT       = (std::numeric_limits<float>::min)();
    // Common Used Variables
    constexpr double MATH_PI        = 3.14159265358979323846264338327950288;
    constexpr double MATH_PI_2      = MATH_PI * 2.0;
    constexpr double MATH_PI_4      = MATH_PI * 4.0;
    constexpr double MATH_1_PI      = 1.0 / MATH_PI;
    constexpr double MATH_2_PI      = 2.0 / MATH_PI;
    constexpr double MATH_2_SQRTPI  = 1.12837916709551257389615890312154517;
    constexpr double EPS            = 1e-9;
    constexpr double SQRT_3         = 1.73205080756887729352744634150587237;
    constexpr double SQRT_2         = 1.41421356237309504880168872420969808;
    constexpr double MATH_LOG2E     = 1.44269504088896340735992468100189214;
    constexpr double MATH_LOG10E    = 0.434294481903251827651128918916605082;
    // For Tetrahedron
    constexpr double ALPHA          = 4.0;
    constexpr double ARTETRAHEDRON  = 8.0 * SQRT_3 / 27.0;
    // For File/Memory
    constexpr std::size_t CHUNK_SIZE = 1024;
    // For CRC Compute
    constexpr std::size_t LUT_SIZE  = 256;
}
