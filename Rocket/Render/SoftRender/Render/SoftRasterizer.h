#pragma once
#include "Render/RenderInfo.h"
#include "Render/SoftTriangle.h"

#include <Eigen/Eigen>

#include <cstdint>
#include <vector>
#include <map>

namespace Rocket {
    struct pos_buf_id {
        int pos_id = 0;
    };

    struct ind_buf_id {
        int ind_id = 0;
    };

    class SoftRasterizer {
    public:
        SoftRasterizer(int32_t w, int32_t h);
        ~SoftRasterizer() = default;

        pos_buf_id LoadPositions(const std::vector<Eigen::Vector3f>& positions);
        ind_buf_id LoadIndices(const std::vector<Eigen::Vector3i>& indices);

        void SetModel(const Eigen::Matrix4f& m) { model_ = m; }
        void SetView(const Eigen::Matrix4f& v) { view_ = v; }
        void SetProjection(const Eigen::Matrix4f& p) { projection_ = p; }

        void SetPixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color);

        void Clear(BufferType buff);

        void Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, RenderPrimitive type);

        std::vector<Eigen::Vector3f>& FrameBuffer() { return frame_buf_; }
        std::vector<float>& DepthBuffer() { return depth_buf_; }
    private:
        void DrawLine(Eigen::Vector3f begin, Eigen::Vector3f end);
        void RasterizeWireframe(const SoftTriangle& t);

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
