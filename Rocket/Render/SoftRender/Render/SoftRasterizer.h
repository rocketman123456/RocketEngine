#pragma once

#include <Eigen/Eigen>

#include <cstdint>
#include <vector>
#include <map>

namespace Rocket {
    class SoftRasterizer {
    public:
        SoftRasterizer(int32_t w, int32_t h);
        ~SoftRasterizer() = default;
    private:
        inline int32_t GetNextId() { return next_id_++; }
        inline int32_t GetIndex(int32_t x, int32_t y) { return (height_-y)*width_ + x; }
    private:
        Eigen::Matrix4f model_;
        Eigen::Matrix4f view_;
        Eigen::Matrix4f projection_;

        std::map<int32_t, std::vector<Eigen::Vector3f>> pos_buf_;
        std::map<int32_t, std::vector<Eigen::Vector3i>> ind_buf_;

        std::vector<Eigen::Vector3f> frame_buf_;
        std::vector<float> depth_buf_;

        int32_t width_ = 0;
        int32_t height_ = 0;
        int32_t next_id_ = 0;
    };
}
