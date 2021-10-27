#include "Geometry/BezierSurface.h"

namespace Rocket {
    namespace Geometry {
        void BezierSurface::Compute(int32_t count) {
            result_.clear();
            result_.resize(count);
            for(int j = 0; j < 4; ++j) {
                control_curve_[j].Compute(count);
            }
            for(int i = 0; i < count; ++i) {
                Eigen::Vector3f cp[4];
                for(int j = 0; j < 4; ++j) {
                    cp[j] = control_curve_[j].GetResult()[i];
                }
                result_[i].clear();
                result_[i].resize(count);
                BezierCurve curve(cp[0], cp[1], cp[2], cp[3]);
                curve.Compute(count);
                for(int32_t k = 0; k < count; k++) {
                    result_[i][k] = curve.GetResult()[k];
                }
            }
        }
    }
}
