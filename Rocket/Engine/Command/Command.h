#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    _Interface_ Command {
    public:
        virtual ~Command() = default;
        virtual void Execute() = 0;
    };
}
