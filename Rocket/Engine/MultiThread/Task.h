#pragma once
#include "Core/Declare.h"

#include <functional>
#include <memory>

namespace Rocket {
    class Task {
    public:
        virtual void Execute() = 0;
        virtual void operator() () = 0;
    };

    using TaskPtr = std::shared_ptr<Task>;
    using TaskFunction = std::function<void(void)>;
} // namespace Rocket
