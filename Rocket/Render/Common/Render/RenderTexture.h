#pragma once

#include <Eigen/Eigen>
#include <algorithm>
#include <vector>
#include <cstdint>

namespace Rocket {
    class RenderTexture {
    public:
        std::vector<Eigen::Vector3f> image_data = {};
        int32_t width = 0;
        int32_t height = 0;

    public:
        RenderTexture(const std::string& name) {
        }

        Eigen::Vector3f getColor(float u, float v) {
            u = std::clamp(u, 0.0f, 1.0f);
            v = std::clamp(v, 0.0f, 1.0f);
            auto u_img = u * width;
            auto v_img = (1 - v) * height;
            auto color = image_data[v_img * width + u_img];
            return Eigen::Vector3f(color[0], color[1], color[2]);
        }
    };
}
