#include "Render/SoftTriangle.h"

namespace Rocket {
    SoftTriangle::SoftTriangle() {
        v[0] << 0, 0, 0;
        v[1] << 0, 0, 0;
        v[2] << 0, 0, 0;

        color[0] << 0.0, 0.0, 0.0;
        color[1] << 0.0, 0.0, 0.0;
        color[2] << 0.0, 0.0, 0.0;

        tex_coords[0] << 0.0, 0.0;
        tex_coords[1] << 0.0, 0.0;
        tex_coords[2] << 0.0, 0.0;
    }

    void SoftTriangle::SetColor(int32_t ind, float r, float g, float b) {
        if ((r < 0.0) || (r > 255.) || (g < 0.0) || (g > 255.) || (b < 0.0) || (b > 255.)) {
            throw std::runtime_error("Invalid color values");
        }

        color[ind] = Eigen::Vector3f((float)r / 255., (float)g / 255., (float)b / 255.);
        return;
    }

    void SoftTriangle::SetTexCoord(int32_t ind, float s, float t) {
        tex_coords[ind] = Eigen::Vector2f(s, t);
    }

    std::array<Eigen::Vector4f, 3> SoftTriangle::ToVector4() const {
        std::array<Eigen::Vector4f, 3> res;
        std::transform(std::begin(v), std::end(v), res.begin(), [](auto& vec) {
            return Eigen::Vector4f(vec.x(), vec.y(), vec.z(), 1.f);
        });
        return res;
    }
}
