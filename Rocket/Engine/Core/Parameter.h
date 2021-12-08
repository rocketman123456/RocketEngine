#pragma once
#include <cstddef>

namespace Rocket {
    // Common Used Variables
    constexpr double MATH_PI        = 3.14159265358979323846264338327950288;
    constexpr double MATH_PI_2      = 1.57079632679489661923132169163975144;
    constexpr double MATH_PI_4      = 0.785398163397448309615660845819875721;
    constexpr double MATH_1_PI      = 0.318309886183790671537767526745028724;
    constexpr double MATH_2_PI      = 0.636619772367581343075535053490057448;
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
}
