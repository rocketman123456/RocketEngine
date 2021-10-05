#pragma once
#include "Vectors.h"

namespace Rocket {
    class Quaternion {
    public:
        Quaternion() = default;
        Quaternion(const Vector4d& q) : quaternion(q) {}
        ~Quaternion() = default;

        Quaternion Multiply(const Quaternion& q);
        double InnerProduct(const Quaternion& q);
        Quaternion Conjugation();

    public:
        Vector4d quaternion = Vector4d::Zero();
    };
}