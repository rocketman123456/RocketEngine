#pragma once
#include "Physics/BasicElement/Triangle.h"
#include "Physics/BasicElement/Vertex.h"

#include <cstdint>
#include <vector>

namespace Rocket {
    class Mesh {
    public:
        Mesh();
        ~Mesh();
    
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
