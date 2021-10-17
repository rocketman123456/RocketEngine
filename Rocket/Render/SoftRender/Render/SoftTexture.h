#pragma once
#include "Render/RenderTexture.h"

#include <algorithm>
#include <vector>
#include <memory>

namespace Rocket {
    class SoftTexture : _implements_ RenderTexture {
    public:
        SoftTexture(const std::string& path, const std::string& name);
        SoftTexture(const std::string& full_path);
        virtual ~SoftTexture() = default;

        virtual Eigen::Vector4f GetColorRGBA(float u, float v) final;
        virtual Eigen::Vector3f GetColorRGB(float u, float v) final;
        virtual Eigen::Vector4f GetPixelRGBA(int32_t x, int32_t y) final;
        virtual Eigen::Vector3f GetPixelRGB(int32_t x, int32_t y) final;

    private:
        std::vector<Eigen::Vector4f> image_data = {};
    };

    using SoftTexturePtr = std::shared_ptr<SoftTexture>;
}
