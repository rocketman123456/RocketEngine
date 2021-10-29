#pragma once
#include "Geometry/BezierCurve.h"

#include <array>
#include <vector>

namespace Rocket {
    namespace Geometry {
        Eigen::Vector3f CalculateBezierSurface(const std::vector<std::vector<Eigen::Vector3f>>& cps, float tx, float ty);
        Eigen::Vector3f CalculateBezierSurfaceRecursice(const std::vector<std::vector<Eigen::Vector3f>>& cps, float tx, float ty);
        void GenerateBezierSurface(const std::vector<Eigen::Vector3f>& vertices, int32_t count_x, int32_t count_y, std::vector<Eigen::Vector3i>& indices);
    }
}
