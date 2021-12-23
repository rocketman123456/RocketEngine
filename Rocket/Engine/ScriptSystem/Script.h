#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    _Interface_ Script {
    public:
        virtual ~Script() = default;

        virtual void Invoke() = 0;
    };
}
