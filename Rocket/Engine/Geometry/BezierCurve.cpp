#include "Geometry/BezierCurve.h"

namespace Rocket {
    namespace Geometry {
        void BezierCurve::Compute(int32_t count) {
            const float dt = 1.0 / (float)count;
            // auto& p_0 = control_point_[0];
            // auto& p_1 = control_point_[1];
            // auto& p_2 = control_point_[2];
            // auto& p_3 = control_point_[3];

            for (float t = 0.0; t <= 1.0; t += dt)  {
                // auto point = 
                //     std::pow(1 - t, 3) * p_0 + 
                //     3 * t * std::pow(1 - t, 2) * p_1 +
                //     3 * std::pow(t, 2) * (1 - t) * p_2 + 
                //     std::pow(t, 3) * p_3;
                auto point = CalculateReCursive(control_point_, t);
                result_.push_back(point);
            }
        }

        Eigen::Vector3f BezierCurve::Calculate(float t) {
            // auto& p_0 = control_point_[0];
            // auto& p_1 = control_point_[1];
            // auto& p_2 = control_point_[2];
            // auto& p_3 = control_point_[3];

            // auto point = 
            //     std::pow(1 - t, 3) * p_0 + 
            //     3 * t * std::pow(1 - t, 2) * p_1 +
            //     3 * std::pow(t, 2) * (1 - t) * p_2 + 
            //     std::pow(t, 3) * p_3;
            auto point = CalculateReCursive(control_point_, t);
            return point;
        }

        Eigen::Vector3f BezierCurve::CalculateReCursive(const std::vector<Eigen::Vector3f>& control_points, float t) {
            if(control_points.size()==2) {
                return (1-t)*control_points[0] + t*control_points[1];
            }
            std::vector<Eigen::Vector3f> vec;
            for(int i = 0; i < control_points.size()-1; i++) {
                vec.push_back((1-t)*control_points[i] + t*control_points[1+i]);
            }
            return CalculateReCursive(vec, t);
        }
    }
}
