#pragma once
#include "Math/Vectors.h"

namespace Rocket {
    class Ray {
    public:
        Vector3d e = Vector3d::Zero();
        Vector3d d = Vector3d::Random().normalized();
        double t = 0;
    };
}
