#include "Geometry/BezierSurface.h"

namespace Rocket {
    namespace Geometry {
        Eigen::Vector3f CalculateBezierSurface(const std::vector<std::vector<Eigen::Vector3f>>& cps, float tx, float ty) {
            std::vector<Eigen::Vector3f> cp_mid;
            for(int32_t i = 0; i < cps.size(); ++i) {
                cp_mid.push_back(CalculateBezierCurve(cps[i], tx));
            }
            return CalculateBezierCurve(cp_mid, ty);
        }

        Eigen::Vector3f CalculateBezierSurfaceRecursice(const std::vector<std::vector<Eigen::Vector3f>>& cps, float tx, float ty) {
            std::vector<Eigen::Vector3f> cp_mid;
            for(int32_t i = 0; i < cps.size(); ++i) {
                cp_mid.push_back(CalculateBezierCurveReCursive(cps[i], tx));
            }
            return CalculateBezierCurveReCursive(cp_mid, ty);
        }

        void GenerateBezierSurface(const std::vector<Eigen::Vector3f>& vertices, int32_t count_x, int32_t count_y, std::vector<Eigen::Vector3i>& indices) {
            assert(vertices.size() == count_x * count_y);

            indices.clear();
            for(int32_t i = 0; i < count_x - 1; ++i) {
                for(int32_t j = 0; j < count_y - 1; ++j) {
                    int32_t i0 = i + j * count_x;
                    int32_t i1 = i + j * count_x + 1;
                    int32_t i2 = i + (j+1) * count_x;
                    int32_t i3 = i + (j+1) * count_x + 1;
                    indices.push_back({i0, i1, i2});
                    indices.push_back({i1, i3, i2});
                }
            }
        }
    }
}
