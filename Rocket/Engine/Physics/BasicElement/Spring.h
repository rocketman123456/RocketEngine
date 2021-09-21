#pragma once
#include "Physics/BasicElement/Edge.h"

#include <memory>

namespace Rocket {
    class Spring {
    public:
        Spring() = default;
    public:
        double force = 0;
        Eigen::Vector3d dir = Eigen::Vector3d::Zero();
    };

    using SpringPtr = std::shared_ptr<Spring>;
}
