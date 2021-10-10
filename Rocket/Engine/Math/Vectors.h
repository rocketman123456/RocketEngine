#pragma once
#include "Math/Parameter.h"

#include <Eigen/Eigen>

namespace Rocket {
    using Vector2f = Eigen::Vector2f;
    using Vector3f = Eigen::Vector3f;
    using Vector4f = Eigen::Vector4f;

    using Vector2d = Eigen::Vector2d;
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;

    namespace Math {
        // Angle between 2 Vector3 Objects
        float AngleBetweenV3(const Vector3f a, const Vector3f b);

        // Projection Calculation of a onto b
        Vector3f ProjV3(const Vector3f a, const Vector3f b);
    }

    namespace Algorithm {
        // A test to see if P1 is on the same side as P2 of a line segment ab
        bool SameSide(Vector3f p1, Vector3f p2, Vector3f a, Vector3f b);

        // Generate a cross produect normal for a triangle
        Vector3f GenTriNormal(Vector3f t1, Vector3f t2, Vector3f t3);

        // Check to see if a Vector3 Point is within a 3 Vector3 Triangle
        bool InTriangle(Vector3f point, Vector3f tri1, Vector3f tri2, Vector3f tri3);
    }
}
