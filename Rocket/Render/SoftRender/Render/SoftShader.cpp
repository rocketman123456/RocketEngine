#include "Render/SoftShader.h"

namespace Rocket {
    void SoftShader::SetModel(const Eigen::Matrix4f& m) {
        model_ = m;
        CalculateMVP();
    }
    
    void SoftShader::SetView(const Eigen::Matrix4f& v) {
        view_ = v;
        CalculateMVP();
    }

    void SoftShader::SetProjection(const Eigen::Matrix4f& p) {
        projection_ = p;
        CalculateMVP();
    }
}
