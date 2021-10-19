#pragma once
#include "Render/RenderInfo.h"
#include "Render/SoftTriangle.h"
#include "Render/SoftShader.h"

#include <Eigen/Eigen>

#include <cstdint>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <tuple>

namespace Rocket {
    struct pos_buf_id {
        int pos_id = 0;
    };

    struct ind_buf_id {
        int ind_id = 0;
    };

    struct col_buf_id {
        int col_id = 0;
    };

    constexpr int32_t FRAME_COUNT = 2;

    class SoftRasterizer {
    public:
        SoftRasterizer(int32_t w, int32_t h);
        ~SoftRasterizer() = default;

        pos_buf_id LoadPositions(const std::vector<Eigen::Vector3f>& positions);
        ind_buf_id LoadIndices(const std::vector<Eigen::Vector3i>& indices);
        col_buf_id LoadColors(const std::vector<Eigen::Vector3f>& colors);
        col_buf_id LoadNormals(const std::vector<Eigen::Vector3f>& normals);

        void UnloadPositions(const pos_buf_id& positions);
        void UnloadIndices(const ind_buf_id& indices);
        void UnloadColors(const col_buf_id& colors);
        void UnloadNormals(const col_buf_id& normals);

        void SetModel(const Eigen::Matrix4f& m) { model_ = m; CalculateMVP(); }
        void SetView(const Eigen::Matrix4f& v) { view_ = v; CalculateMVP(); }
        void SetProjection(const Eigen::Matrix4f& p) { projection_ = p; CalculateMVP(); }

        inline void EnableMsaa() { msaa_ = true; }
        inline void DisableMsaa() { msaa_ = false; }
        inline void EnableWireFrame() { wireframe_ = true; }
        inline void DisableWireFrame() { wireframe_ = false; }
        inline void SetMsaaLevel(int32_t level = 0) { msaa_level_ = level; }

        inline void SetVertexShader(VertexShaderFunc vert_shader) { vertex_shader_ = vert_shader; }
        inline void SetFragmentShader(FragmentShaderFunc frag_shader) { fragment_shader_ = frag_shader; }

        void SetPixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color);
        void SetPixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color);

        void SetTexture(SoftTexturePtr tex) { texture = tex; }

        void NextFrame();
        void Clear(BufferType buff);
        void ClearAll(BufferType buff);

        void Draw(std::vector<SoftTrianglePtr>& TriangleList);
        void Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, RenderPrimitive type);
        void Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, RenderPrimitive type);

        // For Debug or Other use
        void DrawLine3D(const Eigen::Vector3f& begin, const Eigen::Vector3f& end);
        void DrawLines3D(const std::vector<Eigen::Vector3f>& begin, const std::vector<Eigen::Vector3f>& end);
        void DrawPoint3D(const Eigen::Vector3f& point);
        void DrawPoints3D(const std::vector<Eigen::Vector3f>& point);

        void DrawLine3D(const Eigen::Vector3f& begin, const Eigen::Vector3f& end, const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end);
        void DrawLines3D(const std::vector<Eigen::Vector3f>& begin, const std::vector<Eigen::Vector3f>& end, const std::vector<Eigen::Vector3f>& color_begin, const std::vector<Eigen::Vector3f>& color_end);
        void DrawPoint3D(const Eigen::Vector3f& point, const Eigen::Vector3f& color);
        void DrawPoints3D(const std::vector<Eigen::Vector3f>& point, const std::vector<Eigen::Vector3f>& color);

        inline std::vector<Eigen::Vector3f>& FrameBuffer() { return frame_buf_[last_frame_]; }
        inline std::vector<float>& DepthBuffer() { return depth_buf_[last_frame_]; }

    private:
        void RasterizeWireframe(const SoftTriangle& t);
        void RasterizeTriangle(const SoftTriangle& t);
        void RasterizeTriangleWithShader(const SoftTriangle& t, const std::array<Eigen::Vector3f, 3>& view_pos);

        void DrawLine(const Eigen::Vector3f& begin, const Eigen::Vector3f& end);
        void DrawLine(const Eigen::Vector3f& begin, const Eigen::Vector3f& end, const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end);

        // Using Cross-Product Count to Check
        // Must Input 3 vector array
        bool InsideTriangle(float x, float y, const Eigen::Vector3f* _v);
        bool InsideTriangle(float x, float y, const Eigen::Vector4f* _v);
        std::tuple<float, float, float> ComputeBarycentric2D(float x, float y, const Eigen::Vector3f* v);
        std::tuple<float, float, float> ComputeBarycentric2D(float x, float y, const Eigen::Vector4f* v);

        inline void CalculateMVP() { mvp_ = projection_ * view_ * model_; }
        inline int32_t GetNextId() { return next_id_++; }
        inline int32_t GetIndex(int32_t x, int32_t y) { return (height_-y)*width_ + x; }

    private:
        Eigen::Matrix4f model_;
        Eigen::Matrix4f view_;
        Eigen::Matrix4f projection_;
        Eigen::Matrix4f mvp_;

        std::map<int32_t, std::vector<Eigen::Vector3f>> pos_buf_;
        std::map<int32_t, std::vector<Eigen::Vector3i>> ind_buf_;
        std::map<int32_t, std::vector<Eigen::Vector3f>> col_buf_;
        std::map<int32_t, std::vector<Eigen::Vector3f>> nor_buf_;

        int32_t normal_id_ = -1;

        int32_t current_frame_ = 0;
        int32_t last_frame_ = 0;
        // For Multi-Thread Get Framebuffer
        std::atomic<bool> has_finish_ = false;
        
        SoftTexturePtr texture;
        //std::optional<Texture> texture;

        FragmentShaderFunc fragment_shader_;
        VertexShaderFunc vertex_shader_;

        std::vector<Eigen::Vector3f> frame_buf_[FRAME_COUNT];
        std::vector<float> depth_buf_[FRAME_COUNT];

        int32_t width_ = 0;
        int32_t height_ = 0;
        int32_t next_id_ = 0;
        int32_t msaa_level_ = 0;
        int32_t msaa_count_ = 1; // 1 << msaa_level_;
        bool wireframe_ = false;
        bool msaa_ = false;
    };
}
