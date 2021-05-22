#pragma once
#include "Core/Core.h"

#include <functional>

namespace Rocket {
    class SoftRenderDriver {
    public:
        void LoadFragShader();
        void LoadVertShader();
        void LoadGeomShader();
        void InputTriangles();
        void Rasterize();
        void Draw();
    private:
        uint32_t id_ = 0;
        std::function<void()> vert_shader_;
        std::function<void()> frag_shader_;
        std::function<void()> geom_shader_;
    };
}