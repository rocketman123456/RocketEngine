#pragma once
#include <cstdint>

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
    };
}
