#pragma once
#include <Eigen/Eigen>
#include <atomic>

namespace Rocket {
    class Vertex {
    public:
        Vertex() : id(GenerateId()) {}
        Vertex(double x, double y, double z, int32_t type = 0) 
            : id(GenerateId()), type(type), position(x, y, z) {}
        Vertex(const Eigen::Vector3d& pos, int32_t type = 0) 
            : id(GenerateId()), type(type), position(pos) {}
        ~Vertex() = default;

        bool IsCoincidentWith(const Vertex& v);
        // Vertex operator+(const Vertex&);
		// Vertex operator-(const Vertex&);
		// Vertex operator*(const Vertex&);
		// double operator^(const Vertex&);
		// Vertex operator*(double);
		// Vertex operator/(double);
        bool operator==(const Vertex&);

    private:
        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    public:
        int32_t id = 0;
        int32_t type = 0;
        Eigen::Vector3d position = Eigen::Vector3d::Zero();
        bool is_auxiliary = false;
        bool is_visited = false;
        bool is_active = false;
    };
}
