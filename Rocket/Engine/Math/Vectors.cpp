#include "Math/Vectors.h"

namespace Rocket {
    namespace Math {
        // Angle between 2 Vector3 Objects
        float AngleBetweenV3(const Eigen::Vector3f a, const Eigen::Vector3f b) {
            float angle = a.dot(b);
            angle /= (a.norm() * b.norm());
            return angle = acosf(angle);
        }

        // Projection Calculation of a onto b
        Eigen::Vector3f ProjV3(const Eigen::Vector3f a, const Eigen::Vector3f b) {
            auto bn = b.normalized();
            return bn * a.dot(bn);
        }

        Eigen::Vector4f to_vec4(const Eigen::Vector3f &v3, float w) {
            return Eigen::Vector4f(v3.x(), v3.y(), v3.z(), w);
        }
    }

    namespace Algorithm {
        // A test to see if P1 is on the same side as P2 of a line segment ab
        bool SameSide(Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f a, Eigen::Vector3f b) {
            Eigen::Vector3f cp1 = (b - a).cross(p1 - a);
            Eigen::Vector3f cp2 = (b - a).cross(p2 - a);

            if (cp1.dot(cp2) >= 0)
                return true;
            else
                return false;
        }

        // Generate a cross produect normal for a triangle
        Eigen::Vector3f GenTriNormal(Eigen::Vector3f t1, Eigen::Vector3f t2, Eigen::Vector3f t3) {
            Eigen::Vector3f u = t2 - t1;
            Eigen::Vector3f v = t3 - t1;

            Eigen::Vector3f normal = u.cross(v);
            return normal;
        }

        // Check to see if a Vector3 Point is within a 3 Vector3 Triangle
        bool InTriangle(Eigen::Vector3f point, Eigen::Vector3f tri1, Eigen::Vector3f tri2, Eigen::Vector3f tri3) {
            // Test to see if it is within an infinite prism that the triangle outlines.
            bool within_tri_prisim = 
                SameSide(point, tri1, tri2, tri3) &&
                SameSide(point, tri2, tri1, tri3) &&
                SameSide(point, tri3, tri1, tri2);

            // If it isn't it will never be on the triangle
            if (!within_tri_prisim)
                return false;

            // Calulate Triangle's Normal
            Eigen::Vector3f n = GenTriNormal(tri1, tri2, tri3);

            // Project the point onto this normal
            Eigen::Vector3f proj = Math::ProjV3(point, n);

            // If the distance from the triangle to the point is 0
            //	it lies on the triangle
            if (proj.norm() < EPS)
                return true;
            else
                return false;
        }
    }
}