#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <Eigen/Eigen>
#include <memory>

namespace Rocket {
    class Force {

    };

    using ForcePtr = std::shared_ptr<Force>;
}
