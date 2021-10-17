#pragma once
#include "Core/Declare.h"

#include <Eigen/Eigen>
#include <cstdint>

namespace Rocket {
    _Interface_ RenderTexture {
    public:
        int32_t width = 0;
        int32_t height = 0;

    public:
        virtual ~RenderTexture() = default;

        [[nodiscard]] virtual Eigen::Vector4f GetColorRGBA(float u, float v) = 0;
        [[nodiscard]] virtual Eigen::Vector3f GetColorRGB(float u, float v) = 0;
        [[nodiscard]] virtual Eigen::Vector4f GetPixelRGBA(int32_t x, int32_t y) = 0;
        [[nodiscard]] virtual Eigen::Vector3f GetPixelRGB(int32_t x, int32_t y) = 0;

        virtual void BilinearSample() = 0;
        virtual void TrilinearSample() = 0;
    };
}
