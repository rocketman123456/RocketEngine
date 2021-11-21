#pragma once
#include "Core/Declare.h"

#include <functional>

namespace Rocket {
    class Task {
    public:
        virtual void Execute() = 0;
    };
} // namespace Rocket
