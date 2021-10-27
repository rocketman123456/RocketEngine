#pragma once

namespace Rocket {
    namespace Geometry {
        class Box {
        public:
            Box();
        private:
            double x_min_ = 0.0;
            double y_min_ = 0.0;
            double z_min_ = 0.0;

            double x_max_ = 0.0;
            double y_max_ = 0.0;
            double z_max_ = 0.0;
        };
    }
}
