#include "Geometry/BezierSurface.h"

namespace Rocket {
    namespace Geometry {
        void BezierSurface::Compute(int32_t count) {
            count_ = count;
            result_.clear();
            result_.resize(count);
            for(int j = 0; j < 4; ++j) {
                control_curve_[j].Compute(count);
            }
            for(int i = 0; i < count; ++i) {
                std::vector<Eigen::Vector3f> cp;
                for(int j = 0; j < control_curve_.size(); ++j) {
                    cp.push_back(control_curve_[j].GetResult()[i]);
                }
                result_[i].clear();
                result_[i].resize(count);
                BezierCurve curve(cp);
                curve.Compute(count);
                for(int32_t k = 0; k < count; k++) {
                    result_[i][k] = curve.GetResult()[k];
                    vertices_.push_back(curve.GetResult()[k]);
                }
            }
        }

        void BezierSurface::GenerateMesh() {
            for(int32_t i = 0; i < count_ - 1; ++i) {
                for(int32_t j = 0; j < count_ - 1; ++j) {
                    int32_t i0 = i + j * count_;
                    int32_t i1 = i + j * count_ + 1;
                    int32_t i2 = i + (j+1) * count_;
                    int32_t i3 = i + (j+1) * count_ + 1;
                    indices_.push_back({i0, i1, i2});
                    indices_.push_back({i1, i3, i2});
                }
            }
        }
    }
}
