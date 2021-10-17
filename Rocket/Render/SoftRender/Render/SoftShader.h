#pragma once
#include "Core/Declare.h"
#include "Render/RenderShader.h"
#include "Render/SoftTexture.h"

#include <Eigen/Eigen>

#include <cstdint>
#include <unordered_map>

namespace Rocket {
    class SoftShader : _implements_ RenderShader {
    public:
        virtual ~SoftShader() = default;

        void SetModel(const Eigen::Matrix4f& m);
        void SetView(const Eigen::Matrix4f& v);
        void SetProjection(const Eigen::Matrix4f& p);

        inline void CalculateMVP() { mvp_ = projection_ * view_ * model_; }

    private:
        Eigen::Matrix4f model_;
        Eigen::Matrix4f view_;
        Eigen::Matrix4f projection_;
        Eigen::Matrix4f mvp_;

        std::unordered_map<int32_t, std::vector<Eigen::Vector3f>> pos_buf_;
        std::unordered_map<int32_t, std::vector<Eigen::Vector3i>> ind_buf_;
    };

    struct FragmentShaderPayload {
        FragmentShaderPayload() = default;
        FragmentShaderPayload(const Eigen::Vector3f& col, const Eigen::Vector3f& nor,const Eigen::Vector2f& tc, SoftTexture* tex) :
            color(col), normal(nor), tex_coords(tc), texture(tex) {}


        Eigen::Vector3f view_pos = Eigen::Vector3f::Zero();
        Eigen::Vector3f color = Eigen::Vector3f::Zero();
        Eigen::Vector3f normal = Eigen::Vector3f::Zero();
        Eigen::Vector2f tex_coords = Eigen::Vector2f::Zero();
        SoftTexturePtr texture = nullptr;
    };

    struct VertexShaderPayload {
        Eigen::Vector3f position = Eigen::Vector3f::Zero();
    };

    using SoftShaderPtr = std::unique_ptr<SoftShader>;
}
