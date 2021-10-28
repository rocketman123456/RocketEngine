#pragma once
#include "Geometry/BezierCurve.h"

#include <array>
#include <vector>

namespace Rocket {
    namespace Geometry {
        class BezierSurface {
        public:
            BezierSurface(const std::vector<BezierCurve>& curves) : control_curve_(curves) {}

            void Compute(int32_t count);
            void GenerateMesh();

            const std::vector<std::vector<Eigen::Vector3f>>& GetResult() const { return result_; }
            const std::vector<Eigen::Vector3f>& GetVertices() { return vertices_; }
            const std::vector<Eigen::Vector3i>& GetIndices() { return indices_; }
        private:
            std::vector<BezierCurve> control_curve_;
            std::vector<std::vector<Eigen::Vector3f>> result_;
            std::vector<Eigen::Vector3f> vertices_;
            std::vector<Eigen::Vector3i> indices_;

            int32_t count_ = 0;
        };
    }
}
