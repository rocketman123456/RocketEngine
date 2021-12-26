#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    struct ShaderModule {
    };

    _Interface_ Shader {
    public:
        virtual ~Shader() = default;
    };
}