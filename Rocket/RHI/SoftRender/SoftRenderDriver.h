#pragma once
#include "Core/Core.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace Rocket {
    struct SoftRenderBuffer {
    };

    class SoftRenderDriver {
    public:
        void LoadFragmentShader(std::function<void()> shader);
        void LoadVertexShader(std::function<void()> shader);
        void LoadGeometryShader(std::function<void()> shader);
        void LoadComputeShader(std::function<void()> shader);
        void InputTriangles();
        void Rasterize();
        void Draw();
        SoftRenderBuffer* GetRenderBuffer() { return nullptr; }
    private:
        uint32_t id_ = 0;
        std::function<void()> vert_shader_;
        std::function<void()> frag_shader_;
        std::function<void()> geom_shader_;
        std::function<void()> comp_shader_;
        std::unordered_map<std::string, std::unique_ptr<SoftRenderBuffer>> render_buffer_;
    };
}