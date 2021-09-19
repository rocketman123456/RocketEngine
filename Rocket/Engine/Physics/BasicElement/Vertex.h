#pragma once
#include <Eigen/Eigen>
#include <atomic>

namespace Rocket {
    class Vertex {
    public:
        Vertex() : id_(GenerateId()) {}
        Vertex(const Eigen::Vector3d& pos) : id_(GenerateId()), position_(pos) {}
        ~Vertex() = default;

        inline const Eigen::Vector3d& Position() const { return position_; }
        inline Eigen::Vector3d& Position() { return position_; }
        inline int32_t Id() const { return id_; }

    private:
        static int32_t GenerateId() {
            static std::atomic<int32_t> id = 0;
            return id++;
        }

    private:
        int32_t id_ = 0;
        Eigen::Vector3d position_ = Eigen::Vector3d::Zero();
    };
}
