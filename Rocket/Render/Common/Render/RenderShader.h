#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>

namespace Rocket {
    enum class ShaderType : uint32_t {
        VERTEX_SHADER = 0,
        FRAGMENT_SHADER,
        UNKNOWN_SHADER,
    };

    class RenderShader {
    public:
        virtual ~RenderShader() = default;

        inline void SetType(ShaderType type) { type_ = type; }
        inline ShaderType GetType() const { return type_; }
    protected:
        ShaderType type_;
    };

    using RenderShaderPtr = std::unique_ptr<RenderShader>;
}
