#pragma once

#include <Eigen/Eigen>
#include <vector>

namespace Rocket {
    namespace Geometry {
        class BezierCurve {
        public:
            BezierCurve(const std::vector<Eigen::Vector3f>& cp) : control_point_(cp) {}

            void Compute(int32_t count);
            const std::vector<Eigen::Vector3f>& GetResult() const { return result_; }
            Eigen::Vector3f Calculate(float t);
            Eigen::Vector3f CalculateReCursive(const std::vector<Eigen::Vector3f>& control_points, float t);
        private:
            std::vector<Eigen::Vector3f> control_point_;
            std::vector<Eigen::Vector3f> result_;
        };
    }
}