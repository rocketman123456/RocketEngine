#include "Render/SoftTriangle.h"

namespace Rocket {
    SoftTriangle::SoftTriangle() {
        v[0] << 0.0, 0.0, 0.0, 1.0;
        v[1] << 0.0, 0.0, 0.0, 1.0;
        v[2] << 0.0, 0.0, 0.0, 1.0;

        color[0] << 0.0, 0.0, 0.0;
        color[1] << 0.0, 0.0, 0.0;
        color[2] << 0.0, 0.0, 0.0;

        tex_coords[0] << 0.0, 0.0;
        tex_coords[1] << 0.0, 0.0;
        tex_coords[2] << 0.0, 0.0;
    }

    SoftTriangle::SoftTriangle(Geometry::TrianglePtr tri) {
        auto tri_pos0 = tri->vertices[0]->position;
        auto tri_pos1 = tri->vertices[1]->position;
        auto tri_pos2 = tri->vertices[2]->position;
        v[0] = Eigen::Vector4f(tri_pos0[0], tri_pos0[1], tri_pos0[2], 1.0f);
        v[1] = Eigen::Vector4f(tri_pos1[0], tri_pos1[1], tri_pos1[2], 1.0f);
        v[2] = Eigen::Vector4f(tri_pos2[0], tri_pos2[1], tri_pos2[2], 1.0f);

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

    void SoftTriangle::SetNormals(const std::array<Eigen::Vector3f, 3>& normals) {
        normal[0] = normals[0];
        normal[1] = normals[1];
        normal[2] = normals[2];
    }

    void SoftTriangle::SetColors(const std::array<Eigen::Vector3f, 3>& colors) {
        auto first_color = colors[0];
        SetColor(0, colors[0][0], colors[0][1], colors[0][2]);
        SetColor(1, colors[1][0], colors[1][1], colors[1][2]);
        SetColor(2, colors[2][0], colors[2][1], colors[2][2]);
    }

    std::array<Eigen::Vector3f, 3> SoftTriangle::ToVector3() const {
        std::array<Eigen::Vector3f, 3> res;
        std::transform(std::begin(v), std::end(v), res.begin(), [](auto& vec) {
            return Eigen::Vector3f(vec[0], vec[1], vec[2]);
        });
        return res;
    }

    std::array<Eigen::Vector4f, 3> SoftTriangle::ToVector4() const {
        std::array<Eigen::Vector4f, 3> res;
        std::transform(std::begin(v), std::end(v), res.begin(), [](auto& vec) {
            return Eigen::Vector4f(vec[0], vec[1], vec[2], 1.f);
        });
        return res;
    }
}
