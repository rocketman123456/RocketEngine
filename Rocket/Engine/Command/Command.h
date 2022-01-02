#pragma once
#include "Core/MemoryDefine.h"

// #include <string>

namespace Rocket {
    _Interface_ Command {
    public:
        virtual ~Command() = default;
        virtual void Execute() = 0;
        //static const std::string Usage() { return "Usage"; }
    };
}
