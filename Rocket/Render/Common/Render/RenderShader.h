#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "Render/RenderInfo.h"
#include "Log/Log.h"

#include <cstdint>
#include <memory>

namespace Rocket {
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
