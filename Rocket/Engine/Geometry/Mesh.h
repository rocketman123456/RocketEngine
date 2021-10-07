#pragma once
#include "Geometry/Triangle.h"
#include "Geometry/Vertex.h"

#include <cstdint>
#include <vector>

namespace Rocket {
    namespace Geometry {
        class Mesh {
        public:
            Mesh() = default;
            ~Mesh() = default;
        
        private:
            static int32_t GenerateId() {
                static int32_t id = 0;
                return id++;
            }

        public:
            // TODO : remove copy when possible
            std::vector<Vertex> vertices;
            std::vector<Triangle> triangles;
        };
    }
}
