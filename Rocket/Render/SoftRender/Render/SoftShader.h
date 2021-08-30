#pragma once
#include "Core/Declare.h"
#include "Render/RenderShader.h"

#include <Eigen/Eigen>

#include <cstdint>
#include <unordered_map>

namespace Rocket {
    class SoftShader : implements RenderShader {
    public:
        virtual ~SoftShader() = default;

        void SetModel(const Eigen::Matrix4f& m);
        void SetView(const Eigen::Matrix4f& v);
        void SetProjection(const Eigen::Matrix4f& p);

    private:
        Eigen::Matrix4f model_;
        Eigen::Matrix4f view_;
        Eigen::Matrix4f projection_;

        std::unordered_map<int32_t, std::vector<Eigen::Vector3f>> pos_buf_;
        std::unordered_map<int32_t, std::vector<Eigen::Vector3i>> ind_buf_;
    };

    using SoftShaderPtr = std::unique_ptr<SoftShader>;
}
