#pragma once

#include <Eigen/Eigen>
#include <array>
#include <vector>

namespace Rocket {
    namespace Geometry {
        class BezierCurve {
        public:
            BezierCurve(const std::array<Eigen::Vector3f, 4>& cp) : control_point_(cp) {}
            BezierCurve(Eigen::Vector3f cp0, Eigen::Vector3f cp1, Eigen::Vector3f cp2, Eigen::Vector3f cp3) : control_point_({cp0, cp1, cp2, cp3}) {}

            void Compute(int32_t count);
            const std::vector<Eigen::Vector3f>& GetResult() const { return result_; }
            Eigen::Vector3f Calculate(float t);
            Eigen::Vector3f CalculateReCursive(const std::vector<Eigen::Vector3f>& control_points, float t);
        private:
            std::array<Eigen::Vector3f, 4> control_point_;
            std::vector<Eigen::Vector3f> result_;
        };
    }
}