#pragma once
#include "Vectors.h"

namespace Rocket {
    class Quaternion {
    public:
        Quaternion() = default;
        Quaternion(const Eigen::Vector4d& q) : quaternion(q) {}
        ~Quaternion() = default;

        Quaternion Multiply(const Quaternion& q);
        double InnerProduct(const Quaternion& q);
        Quaternion Conjugation();

    public:
        Eigen::Vector4d quaternion = Eigen::Vector4d::Zero();
    };
}