#pragma once
#include "Render/RenderInfo.h"
#include "Render/RenderInterface.h"
#include "Render/SoftTriangle.h"
#include "Render/SoftShader.h"

#include <Eigen/Eigen>

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>
#include <tuple>

namespace Rocket {
    struct pos_buf_id { int pos_id = 0; };
    struct ind_buf_id { int ind_id = 0; };
    struct col_buf_id { int col_id = 0; };

    constexpr int32_t FRAME_COUNT = 2;

    class SoftRasterizer : _implements_ RenderInterface {
        using Vector3fVec = std::vector<Eigen::Vector3f>;
        using Vector3iVec = std::vector<Eigen::Vector3i>;
    public:
        SoftRasterizer(int32_t w, int32_t h);
        virtual ~SoftRasterizer() = default;

        pos_buf_id LoadPositions(const Vector3fVec& positions);
        ind_buf_id LoadIndices(const Vector3iVec& indices);
        col_buf_id LoadColors(const Vector3fVec& colors);
        col_buf_id LoadNormals(const Vector3fVec& normals);

        inline void UnloadPositions(const pos_buf_id& positions) { pos_buf_.erase(pos_buf_.find(positions.pos_id)); }
        inline void UnloadIndices(const ind_buf_id& indices) { ind_buf_.erase(ind_buf_.find(indices.ind_id)); }
        inline void UnloadColors(const col_buf_id& colors) { col_buf_.erase(col_buf_.find(colors.col_id)); }
        inline void UnloadNormals(const col_buf_id& normals) { nor_buf_.erase(nor_buf_.find(normals.col_id)); }

        inline void SetModel(const Eigen::Matrix4f& m) { model_ = m; CalculateMVP(); }
        inline void SetView(const Eigen::Matrix4f& v) { view_ = v; CalculateMVP(); }
        inline void SetProjection(const Eigen::Matrix4f& p) { projection_ = p; CalculateMVP(); }
        inline void CalculateMVP() { mvp_ = projection_ * view_ * model_; }

        inline void EnableMsaa() { msaa_ = true; }
        inline void DisableMsaa() { msaa_ = false; }
        inline void EnableWireFrame() { wireframe_ = true; }
        inline void DisableWireFrame() { wireframe_ = false; }
        inline void SetMsaaLevel(int32_t level = 0) { msaa_level_ = level; }
        inline void EnableShader() { with_shader_ = true; }
        inline void DisableShader() { with_shader_ = false; }
        inline auto GetWidth() { return width_; }
        inline auto GetHeight() { return height_; }
        inline void SetVertexShader(VertexShaderFunc vert_shader) { vertex_shader_ = vert_shader; }
        inline auto GetVertexShader() { return vertex_shader_; }
        inline void SetFragmentShader(FragmentShaderFunc frag_shader) { fragment_shader_ = frag_shader; }
        inline auto GetFragmentShader() { return fragment_shader_; }
        inline void SetTexture(SoftTexturePtr tex) { texture_ = tex; }
        inline auto GetTexture() { return texture_; }

        inline std::vector<Eigen::Vector3f>& FrameBufferNext() { return frame_buf_[current_frame_]; }
        inline std::vector<float>& DepthBufferNext() { return depth_buf_[current_frame_]; } 
        inline std::vector<Eigen::Vector3f>& FrameBuffer() { return frame_buf_[last_frame_]; }
        inline std::vector<float>& DepthBuffer() { return depth_buf_[last_frame_]; }

        inline void NextFrame() { last_frame_ = current_frame_++; current_frame_ = current_frame_%FRAME_COUNT; }

        void SetPixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color);
        void SetPixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color);

        void Clear(BufferType buff);
        void ClearAll(BufferType buff);

        void Draw(const SoftTrianglePtr& t);
        void Draw(const std::vector<SoftTrianglePtr>& TriangleList);
        void Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, RenderPrimitive type);
        void Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, RenderPrimitive type);

        // For Debug or Other use
        void DrawLine3D(const Eigen::Vector3f& begin, const Eigen::Vector3f& end);
        void DrawLines3D(const Vector3fVec& begin, const Vector3fVec& end);
        void DrawPoint3D(const Eigen::Vector3f& point);
        void DrawPoints3D(const Vector3fVec& point);

        void DrawLine3D(const Eigen::Vector3f& begin, const Eigen::Vector3f& end, const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end);
        void DrawLines3D(const Vector3fVec& begin, const Vector3fVec& end, const Vector3fVec& color_begin, const Vector3fVec& color_end);
        void DrawPoint3D(const Eigen::Vector3f& point, const Eigen::Vector3f& color);
        void DrawPoints3D(const Vector3fVec& point, const Vector3fVec& color);

        void DrawLine(const Eigen::Vector3f& begin, const Eigen::Vector3f& end, const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end);

    private:
        // Using Cross-Product Count to Check
        // Must Input 3 vector array
        template<typename T>    // Should Be Eigen::Vector3f or Eigen::Vector4f
        bool InsideTriangle(float x, float y, const T* _v);
        template<typename T>    // Should Be Eigen::Vector3f or Eigen::Vector4f
        std::tuple<float, float, float> ComputeBarycentric2D(float x, float y, const T* v);

    private:
        void RasterizeWireframe(const SoftTriangle& t);
        void RasterizeTriangle(const SoftTriangle& t, const std::array<Eigen::Vector3f, 3>& view_pos = {});

        Eigen::Vector3f CalculateColor(
            const SoftTriangle& t, int x, int y, float& minDepth, 
            const Eigen::Vector2f& pos, 
            const std::array<Eigen::Vector4f, 3>& v);
        Eigen::Vector3f CalculateColorWithShader(
            const SoftTriangle& t, int x, int y, float& minDepth, 
            const Eigen::Vector2f& pos, 
            const std::array<Eigen::Vector4f, 3>& v, 
            const std::array<Eigen::Vector3f, 3>& view_pos);

        inline int32_t GetNextId() { return next_id_++; }
        inline int32_t GetIndex(int32_t x, int32_t y) { return (height_-y)*width_ + x; }
        float FindDepth(int32_t x, int32_t y);

    private:
        Eigen::Matrix4f model_;
        Eigen::Matrix4f view_;
        Eigen::Matrix4f projection_;
        Eigen::Matrix4f mvp_;

        std::unordered_map<int32_t, Vector3fVec> pos_buf_;
        std::unordered_map<int32_t, Vector3iVec> ind_buf_;
        std::unordered_map<int32_t, Vector3fVec> col_buf_;
        std::unordered_map<int32_t, Vector3fVec> nor_buf_;

        int32_t normal_id_ = -1;

        // draw on current frame, show last frame
        int32_t current_frame_ = 0;
        int32_t last_frame_ = 0;
        // For Multi-Thread Get Framebuffer
        std::atomic<bool> has_finish_[FRAME_COUNT] = { false };

        std::optional<SoftTexturePtr> texture_;

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
        bool with_shader_ = false;
    };

    template<typename T>
    bool SoftRasterizer::InsideTriangle(float x, float y, const T _v[]) {
        Eigen::Vector2f screen_point = Eigen::Vector2f(x, y);

        Eigen::Vector2f v[3];
        Eigen::Vector2f dir[3];

        for(int32_t i = 0; i < 3; ++i)
            v[i] = _v[(i+1) % 3].head(2) - _v[i].head(2);
        for(int32_t i = 0; i < 3; ++i)
            dir[i] = screen_point - _v[i].head(2);

        int32_t count = 0;
        float result = 0;
        for(int32_t i = 0; i < 3; ++i) {
            result = v[i][0] * dir[i][1] - v[i][1] * dir[i][0];
            if(result > 0) {
                count++;
            } else if(result < 0) {
                count--;
            }
        }
        return count == 3 || count == -3;
    }

    template<typename T>
    std::tuple<float, float, float> SoftRasterizer::ComputeBarycentric2D(float x, float y, const T _v[]) {
        float c1 = (x*(_v[1].y() - _v[2].y()) + (_v[2].x() - _v[1].x())*y + _v[1].x()*_v[2].y() - _v[2].x()*_v[1].y()) / 
            (_v[0].x()*(_v[1].y() - _v[2].y()) + (_v[2].x() - _v[1].x())*_v[0].y() + _v[1].x()*_v[2].y() - _v[2].x()*_v[1].y());
        float c2 = (x*(_v[2].y() - _v[0].y()) + (_v[0].x() - _v[2].x())*y + _v[2].x()*_v[0].y() - _v[0].x()*_v[2].y()) / 
            (_v[1].x()*(_v[2].y() - _v[0].y()) + (_v[0].x() - _v[2].x())*_v[1].y() + _v[2].x()*_v[0].y() - _v[0].x()*_v[2].y());
        float c3 = (x*(_v[0].y() - _v[1].y()) + (_v[1].x() - _v[0].x())*y + _v[0].x()*_v[1].y() - _v[1].x()*_v[0].y()) / 
            (_v[2].x()*(_v[0].y() - _v[1].y()) + (_v[1].x() - _v[0].x())*_v[2].y() + _v[0].x()*_v[1].y() - _v[1].x()*_v[0].y());
        return { c1, c2, c3 };
    }
}
