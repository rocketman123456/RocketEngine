#pragma once
#include "Core/Declare.h"
#include "Render/RenderBuffer.h"

#include <Eigen/Eigen>

#include <cstdint>
#include <vector>

namespace Rocket {
    class SoftBuffer : _implements_ RenderBuffer {
    public:
        virtual ~SoftBuffer() = default;

        void SetPixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color);

    private:
        std::vector<Eigen::Vector3f> frame_buf_;
        std::vector<float> depth_buf_;

        int32_t width_;
        int32_t height_;
    };

    using SoftBufferPtr = std::unique_ptr<SoftBuffer>;
}
