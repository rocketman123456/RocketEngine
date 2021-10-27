#pragma once
#include "Geometry/BezierCurve.h"

#include <array>
#include <vector>

namespace Rocket {
    namespace Geometry {
        class BezierSurface {
        public:
            BezierSurface(BezierCurve c1, BezierCurve c2, BezierCurve c3, BezierCurve c4) 
                : control_curve_({c1, c2, c3, c4}) {}
            
            void Compute(int32_t count);
            const std::vector<std::vector<Eigen::Vector3f>>& GetResult() const { return result_; }
        private:
            std::array<BezierCurve, 4> control_curve_;
            std::vector<std::vector<Eigen::Vector3f>> result_;
        };
    }
}
