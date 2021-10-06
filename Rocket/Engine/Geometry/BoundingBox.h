#pragma once
#include "Core/Declare.h"

namespace Rocket {
    class BoundingBox2D {
    public:
        double x_min = 0;
        double y_min = 0;
        double x_max = 0;
        double y_max = 0;
    };

    class BoundingBox2D {
    public:
        double x_min = 0;
        double y_min = 0;
        double z_min = 0;
        double x_max = 0;
        double y_max = 0;
        double z_max = 0;
    };
}
