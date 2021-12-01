#pragma once

#include <Eigen/Eigen>

namespace Rocket {
    // From Quake 3 Source Code
    float CarmSqrt(float x);

    template<typename T>
    T Interpolate(const Eigen::Vector3f& a, const std::array<T, 3> v) {
        return a[0] * v[0] + a[1] * v[1] + a[2] * v[2]; 
    }
}
