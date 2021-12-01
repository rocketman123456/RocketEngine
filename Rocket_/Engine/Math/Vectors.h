#pragma once
#include "Math/Parameter.h"
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <Eigen/Eigen>

namespace Rocket {
    // using Vector2f = Eigen::Vector2f;
    // using Vector3f = Eigen::Vector3f;
    // using Vector4f = Eigen::Vector4f;

    // using Vector2d = Eigen::Vector2d;
    // using Vector3d = Eigen::Vector3d;
    // using Vector4d = Eigen::Vector4d;

    namespace Math {
        // Angle between 2 Vector3 Objects
        float AngleBetweenV3(const Eigen::Vector3f& a, const Eigen::Vector3f& b);

        // Projection Calculation of a onto b
        Eigen::Vector3f ProjV3(const Eigen::Vector3f& a, const Eigen::Vector3f& b);

        Eigen::Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f);

        double PointLineDistance(const Eigen::Vector3f& point, const Eigen::Vector3f& line_start, const Eigen::Vector3f& line_end);

        // A test to see if P1 is on the same side as P2 of a line segment ab
        bool SameSide(const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& a, const Eigen::Vector3f& b);

        // Generate a cross produect normal for a triangle
        Eigen::Vector3f GenTriNormal(const Eigen::Vector3f& t1, const Eigen::Vector3f& t2, const Eigen::Vector3f& t3);

        // Check to see if a Vector3 Point is within a 3 Vector3 Triangle
        bool InTriangle(const Eigen::Vector3f& point, const Eigen::Vector3f& tri1, const Eigen::Vector3f& tri2, const Eigen::Vector3f& tri3);
    }
}
