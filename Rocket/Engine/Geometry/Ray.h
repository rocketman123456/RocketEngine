#pragma once
#include "Memory/MemoryDefine.h"
#include "Math/Vectors.h"

namespace Rocket {
    namespace Geometry {
        class Ray {
        public:
            Eigen::Vector3d e = Eigen::Vector3d::Zero();
            Eigen::Vector3d d = Eigen::Vector3d::Random().normalized();
            double t = 0;
        };
    }
}
