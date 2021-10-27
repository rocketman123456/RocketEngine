#include "Math/Quaternion.h"

namespace Rocket {
    Quaternion Quaternion::Multiply(const Quaternion& q_) {
        auto p0 = quaternion[0];
        auto q0 = q_.quaternion[0];
        auto p = quaternion.block<3,1>(1, 0);
        auto q = q_.quaternion.block<3,1>(1, 0);

        double result_1 = p0 * q0 - p.dot(q);
        Eigen::Vector3d result_2 = p0 * q + q0 * p + p.cross(q);

        return { {result_1, result_2[0], result_2[1], result_2[2]} };
    }

    double Quaternion::InnerProduct(const Quaternion& q_) {
        auto p0 = quaternion[0];
        auto q0 = q_.quaternion[0];
        auto p = quaternion.block<3,1>(1, 0);
        auto q = q_.quaternion.block<3,1>(1, 0);

        return p0 * q0 + p.dot(q);
    }

    Quaternion Quaternion::Conjugation() {
        auto p0 = quaternion[0];
        auto p = quaternion.block<3,1>(1, 0);
        return { {p0, -p[0], -p[1], -p[2]} };
    }
}
