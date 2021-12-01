#pragma once
#include "Memory/MemoryDefine.h"

#include <Eigen/Eigen>
#include <vector>

namespace Rocket {
    namespace Geometry {
        Eigen::Vector3f CalculateBezierCurve(const std::vector<Eigen::Vector3f>& cp, float t);
        Eigen::Vector3f CalculateBezierCurveReCursive(const std::vector<Eigen::Vector3f>& cp, float t);
        void SubdividingBezierCurve(const std::vector<Eigen::Vector3f>& cp, double u, std::vector<Eigen::Vector3f>& cp_left, std::vector<Eigen::Vector3f>& cp_right);
        void UpgradeBezierCurve(const std::vector<Eigen::Vector3f>& cp, std::vector<Eigen::Vector3f>& cp_new);
    }
}