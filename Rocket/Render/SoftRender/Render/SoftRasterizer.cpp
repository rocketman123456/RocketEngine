#include "Render/SoftRasterizer.h"
#include "Math/Parameter.h"
#include "Math/ComonMath.h"
#include "Log/Log.h"

#include <cfloat>
#include <iostream>

namespace Rocket {
    static Eigen::Vector4f to_vec4(const Eigen::Vector3f &v3, float w = 1.0f) {
        return Eigen::Vector4f(v3.x(), v3.y(), v3.z(), w);
    }

    SoftRasterizer::SoftRasterizer(int w, int h) : width_(w), height_(h) {
        for(int i = 0; i < FRAME_COUNT; ++i) {
            frame_buf_[i].resize(w * h);
            depth_buf_[i].resize(w * h);
        }
    }

    pos_buf_id SoftRasterizer::LoadPositions(const std::vector<Eigen::Vector3f> &positions) {
        auto id = GetNextId();
        pos_buf_.emplace(id, positions);
        return {id};
    }

    ind_buf_id SoftRasterizer::LoadIndices(const std::vector<Eigen::Vector3i> &indices) {
        auto id = GetNextId();
        ind_buf_.emplace(id, indices);
        return {id};
    }

    col_buf_id SoftRasterizer::LoadColors(const std::vector<Eigen::Vector3f>& colors) {
        auto id = GetNextId();
        col_buf_.emplace(id, colors);
        return {id};
    }

    col_buf_id SoftRasterizer::LoadNormals(const std::vector<Eigen::Vector3f>& normals) {
        auto id = GetNextId();
        nor_buf_.emplace(id, normals);
        normal_id_ = id;
        return {id};
    }

    void SoftRasterizer::UnloadPositions(const pos_buf_id& positions) {
        pos_buf_.erase(pos_buf_.find(positions.pos_id));
    }

    void SoftRasterizer::UnloadIndices(const ind_buf_id& indices) {
        ind_buf_.erase(ind_buf_.find(indices.ind_id));
    }

    void SoftRasterizer::UnloadColors(const col_buf_id& colors) {
        col_buf_.erase(col_buf_.find(colors.col_id));
    }

    void SoftRasterizer::UnloadNormals(const col_buf_id& normals) {
        nor_buf_.erase(nor_buf_.find(normals.col_id));
    }

    bool SoftRasterizer::InsideTriangle(float x, float y, const Eigen::Vector3f* _v) {
        Eigen::Vector2f screen_point = Eigen::Vector2f(x, y);

        Eigen::Vector2f v[3];
        v[0] = _v[1].head(2) - _v[0].head(2);
        v[1] = _v[2].head(2) - _v[1].head(2);
        v[2] = _v[0].head(2) - _v[2].head(2);

        Eigen::Vector2f dir[3];
        dir[0] = screen_point - _v[0].head(2);
        dir[1] = screen_point - _v[1].head(2);
        dir[2] = screen_point - _v[2].head(2);

        int count = 0;
        for(int i = 0; i < 3; ++i) {
            float result = v[i][0] * dir[i][1] - v[i][1] * dir[i][0];
            if(result > 0) {
                count++;
            }
            else if(result < 0) {
                count--;
            }
        }

        return count == 3 || count == -3;
    }

    bool SoftRasterizer::InsideTriangle(float x, float y, const Eigen::Vector4f* _v) {
        Eigen::Vector2f screen_point = Eigen::Vector2f(x, y);

        Eigen::Vector2f v[3];
        v[0] = _v[1].head(2) - _v[0].head(2);
        v[1] = _v[2].head(2) - _v[1].head(2);
        v[2] = _v[0].head(2) - _v[2].head(2);

        Eigen::Vector2f dir[3];
        dir[0] = screen_point - _v[0].head(2);
        dir[1] = screen_point - _v[1].head(2);
        dir[2] = screen_point - _v[2].head(2);

        int count = 0;
        for(int i = 0; i < 3; ++i) {
            float result = v[i][0] * dir[i][1] - v[i][1] * dir[i][0];
            if(result > 0) {
                count++;
            }
            else if(result < 0) {
                count--;
            }
        }

        return count == 3 || count == -3;
    }

    std::tuple<float, float, float> SoftRasterizer::ComputeBarycentric2D(float x, float y, const Eigen::Vector3f* v) {
        float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / 
            (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
        float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / 
            (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
        float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / 
            (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
        return {c1,c2,c3};
    }

    std::tuple<float, float, float> SoftRasterizer::ComputeBarycentric2D(float x, float y, const Eigen::Vector4f* v) {
        float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / 
            (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
        float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / 
            (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
        float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / 
            (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
        return {c1,c2,c3};
    }

    void SoftRasterizer::DrawLine(
        const Eigen::Vector3f& begin, const Eigen::Vector3f& end, 
        const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end) {
        
        auto x1 = begin.x();
        auto y1 = begin.y();
        auto x2 = end.x();
        auto y2 = end.y();

        Eigen::Vector3f color_delta = {0, 0, 0};
        Eigen::Vector3f line_color = {0, 0, 0};
        float length = (end - begin).norm();

        int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

        dx = x2 - x1;
        dy = y2 - y1;
        dx1 = fabs(dx);
        dy1 = fabs(dy);
        px = 2 * dy1 - dx1;
        py = 2 * dx1 - dy1;

        if (dy1 <= dx1) {
            if (dx >= 0) {
                x = x1;
                y = y1;
                xe = x2;
                line_color = color_begin;
                color_delta = color_end - color_begin;
            }
            else {
                x = x2;
                y = y2;
                xe = x1;
                line_color = color_end;
                color_delta = color_begin - color_end;
            }
            if(length > 1e-6) {
                color_delta = color_delta / length;
            }
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            SetPixel(point, line_color);
            for (i = 0; x < xe; i++) {
                x = x + 1;
                if (px < 0) {
                    px = px + 2 * dy1;
                }
                else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        y = y + 1;
                    }
                    else {
                        y = y - 1;
                    }
                    px = px + 2 * (dy1 - dx1);
                }
                Eigen::Vector2i point(x, y);
                line_color += color_delta;
                SetPixel(point, line_color);
            }
        }
        else {
            if (dy >= 0) {
                x = x1;
                y = y1;
                ye = y2;
                line_color = color_begin;
                color_delta = color_end - color_begin;

            }
            else {
                x = x2;
                y = y2;
                ye = y1;
                line_color = color_end;
                color_delta = color_begin - color_end;
            }
            if(length > 1e-6) {
                color_delta = color_delta / length;
            }
            Eigen::Vector2i point(x, y);
            SetPixel(point, line_color);
            for (i = 0; y < ye; i++) {
                y = y + 1;
                if (py <= 0) {
                    py = py + 2 * dx1;
                }
                else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        x = x + 1;
                    }
                    else {
                        x = x - 1;
                    }
                    py = py + 2 * (dx1 - dy1);
                }
                Eigen::Vector2i point(x, y);
                line_color += color_delta;
                SetPixel(point, line_color);
            }
        }
    }

    // Bresenham's line drawing algorithm
    // Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
    void SoftRasterizer::DrawLine(const Eigen::Vector3f& begin, const Eigen::Vector3f& end) {
        DrawLine(begin, end, {255,0,0}, {0,255,0});
    }

    void SoftRasterizer::NextFrame() {
        last_frame_ = current_frame_;
        current_frame_++;
        current_frame_ = current_frame_ % FRAME_COUNT;
    }

    void SoftRasterizer::DrawLine3D(const Eigen::Vector3f& begin, const Eigen::Vector3f& end) {
        DrawLine3D(begin, end, {255, 255, 255}, {255, 255, 255});
    }

    void SoftRasterizer::DrawLines3D(const std::vector<Eigen::Vector3f>& begin, const std::vector<Eigen::Vector3f>& end) {
        assert(begin.size() == end.size());

        for(int32_t i = 0; i < begin.size(); ++i) {
            DrawLine3D(begin[i], end[i], {255, 255, 255}, {255, 255, 255});
        }
    }

    void SoftRasterizer::DrawPoint3D(const Eigen::Vector3f& point) {
        Eigen::Vector3f point_color = {255, 255, 255};
        DrawPoint3D(point, point_color);
    }

    void SoftRasterizer::DrawPoints3D(const std::vector<Eigen::Vector3f>& point) {
        Eigen::Vector3f point_color = {255, 255, 255};
        for(auto single_point : point) {
            DrawPoint3D(single_point, point_color);
        }
    }

    void SoftRasterizer::DrawLine3D(
        const Eigen::Vector3f& begin, const Eigen::Vector3f& end, 
        const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end) {

        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        Eigen::Vector4f v[] = {
            mvp_ * to_vec4(begin, 1.0f),
            mvp_ * to_vec4(end, 1.0f),
        };
        // Homogeneous division
        for (auto &vec : v) {
            vec /= vec.w();
        }
        // Viewport transformation
        for (auto &vert : v) {
            vert.x() = 0.5 * width_ * (vert.x() + 1.0);
            vert.y() = 0.5 * height_ * (vert.y() + 1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        DrawLine(v[0].head<3>(), v[1].head<3>(), color_begin, color_end);
    }

    void SoftRasterizer::DrawLines3D(
        const std::vector<Eigen::Vector3f>& begin, const std::vector<Eigen::Vector3f>& end, 
        const std::vector<Eigen::Vector3f>& color_begin, const std::vector<Eigen::Vector3f>& color_end) {
        
        assert(begin.size() == end.size());
        assert(color_begin.size() == color_end.size());
        assert(begin.size() == color_begin.size());

        for(int32_t i = 0; i < begin.size(); ++i) {
            DrawLine3D(begin[i], end[i], color_begin[i], color_end[i]);
        }
    }
    
    void SoftRasterizer::DrawPoint3D(const Eigen::Vector3f& point, const Eigen::Vector3f& color) {
        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        Eigen::Vector4f v[] = {
            mvp_ * to_vec4(point, 1.0f),
        };
        // Homogeneous division
        for (auto &vec : v) {
            vec /= vec.w();
        }
        // Viewport transformation
        for (auto &vert : v) {
            vert.x() = 0.5 * width_ * (vert.x() + 1.0);
            vert.y() = 0.5 * height_ * (vert.y() + 1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        DrawLine(v[0].head<3>(), v[0].head<3>(), color, color);
    }

    void SoftRasterizer::DrawPoints3D(const std::vector<Eigen::Vector3f>& point, const std::vector<Eigen::Vector3f>& color) {
        assert(point.size() == color.size());
        
        for(int32_t i = 0; i < point.size(); i++) {
            DrawPoint3D(point[i], color[i]);
        }
    }

    void SoftRasterizer::Draw(std::vector<SoftTrianglePtr>& TriangleList) {
        float f1 = (50 - 0.1) / 2.0;
        float f2 = (50 + 0.1) / 2.0;
        
        int32_t count = 0;
        for (const auto& t:TriangleList) {
            //RK_INFO(Render, "Render {} Triangle", count);
            std::cout << "Render Tirangle: " << count++ << std::endl;

            SoftTriangle& newtri = *t.get();

            std::array<Eigen::Vector4f, 3> mm {
                (view_ * model_ * t->v[0]),
                (view_ * model_ * t->v[1]),
                (view_ * model_ * t->v[2])
            };

            std::array<Eigen::Vector3f, 3> viewspace_pos;

            std::transform(mm.begin(), mm.end(), viewspace_pos.begin(), [](auto& v) {
                return v.template head<3>();
            });

            Eigen::Vector4f v[] = {
                mvp_ * t->v[0],
                mvp_ * t->v[1],
                mvp_ * t->v[2]
            };

            //Homogeneous division
            for (auto& vec : v) {
                vec.x()/=vec.w();
                vec.y()/=vec.w();
                vec.z()/=vec.w();
            }

            Eigen::Matrix4f inv_trans = (view_ * model_).inverse().transpose();
            Eigen::Vector4f n[] = {
                    inv_trans * to_vec4(t->normal[0], 0.0f),
                    inv_trans * to_vec4(t->normal[1], 0.0f),
                    inv_trans * to_vec4(t->normal[2], 0.0f)
            };

            //Viewport transformation
            for (auto & vert : v)
            {
                vert.x() = 0.5 * width_ * (vert.x() + 1.0);
                vert.y() = 0.5 * height_ * (vert.y() + 1.0);
                vert.z() = vert.z() * f1 + f2;
            }

            for (int i = 0; i < 3; ++i)
            {
                //screen space coordinates
                newtri.SetVertex(i, v[i]);
            }

            for (int i = 0; i < 3; ++i)
            {
                //view space normal
                newtri.SetNormal(i, n[i].head<3>());
            }

            newtri.SetColor(0, 148,121.0,92.0);
            newtri.SetColor(1, 148,121.0,92.0);
            newtri.SetColor(2, 148,121.0,92.0);

            // Also pass view space vertice position
            RasterizeTriangleWithShader(newtri, viewspace_pos);
        }
    }

    void SoftRasterizer::Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, RenderPrimitive type) {
        if (type != RenderPrimitive::TRIANGLE) {
            throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
        }

        auto &buf = pos_buf_[pos_buffer.pos_id];
        auto &ind = ind_buf_[ind_buffer.ind_id];

        float f1 = (50 - 0.1) / 2.0;
        float f2 = (50 + 0.1) / 2.0;

        for (auto &i : ind) {
            SoftTriangle t;

            Eigen::Vector4f v[] = {
                mvp_ * to_vec4(buf[i[0]], 1.0f),
                mvp_ * to_vec4(buf[i[1]], 1.0f),
                mvp_ * to_vec4(buf[i[2]], 1.0f)
            };

            // Homogeneous division
            for (auto &vec : v) {
                vec /= vec.w();
            }
            // Viewport transformation
            for (auto &vert : v) {
                vert.x() = 0.5 * width_ * (vert.x() + 1.0);
                vert.y() = 0.5 * height_ * (vert.y() + 1.0);
                vert.z() = vert.z() * f1 + f2;
            }

            for (int i = 0; i < 3; ++i) {
                t.SetVertex(i, v[i]);
            }

            t.SetColor(0, 255.0, 0.0, 0.0);
            t.SetColor(1, 0.0, 255.0, 0.0);
            t.SetColor(2, 0.0, 0.0, 255.0);

            if(wireframe_) {
                RasterizeWireframe(t);
            }
            else {
                RasterizeTriangle(t);
            }
        }
    }

    void SoftRasterizer::Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, RenderPrimitive type) {
        if (type != RenderPrimitive::TRIANGLE) {
            throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
        }

        auto& buf = pos_buf_[pos_buffer.pos_id];
        auto& ind = ind_buf_[ind_buffer.ind_id];
        auto& col = col_buf_[col_buffer.col_id];

        float f1 = (50 - 0.1) / 2.0;
        float f2 = (50 + 0.1) / 2.0;

        for (auto& i : ind)
        {
            SoftTriangle t;
            Eigen::Vector4f v[] = {
                    mvp_ * to_vec4(buf[i[0]], 1.0f),
                    mvp_ * to_vec4(buf[i[1]], 1.0f),
                    mvp_ * to_vec4(buf[i[2]], 1.0f)
            };
            // Homogeneous division
            for (auto& vec : v) {
                vec /= vec.w();
            }
            // Viewport transformation
            for (auto & vert : v) {
                vert.x() = 0.5 * width_ * (vert.x() + 1.0);
                vert.y() = 0.5 * height_ * (vert.y() + 1.0);
                vert.z() = vert.z() * f1 + f2;
            }

            for (int i = 0; i < 3; ++i) {
                t.SetVertex(i, v[i]);
            }

            auto col_x = col[i[0]];
            auto col_y = col[i[1]];
            auto col_z = col[i[2]];

            t.SetColor(0, col_x[0], col_x[1], col_x[2]);
            t.SetColor(1, col_y[0], col_y[1], col_y[2]);
            t.SetColor(2, col_z[0], col_z[1], col_z[2]);

            if(wireframe_) {
                RasterizeWireframe(t);
            }
            else {
                RasterizeTriangle(t);
            }
        }
    }

    void SoftRasterizer::RasterizeWireframe(const SoftTriangle &t) {
        DrawLine(t.c3f(), t.a3f());
        DrawLine(t.c3f(), t.b3f());
        DrawLine(t.b3f(), t.a3f());
    }

    void SoftRasterizer::RasterizeTriangle(const SoftTriangle &t) {
        auto v = t.ToVector4();
    
        // Find out the bounding box of current triangle.
        // iterate through the pixel and find if the current pixel is inside the triangle
        Eigen::Vector2i minAabb; // left bottom
        Eigen::Vector2i maxAabb; // right up

        minAabb[0] = std::min(v[0][0],std::min(v[1][0], v[2][0]));
        maxAabb[0] = std::max(v[0][0],std::max(v[1][0], v[2][0]));
        minAabb[1] = std::min(v[0][1],std::min(v[1][1], v[2][1]));
        maxAabb[1] = std::max(v[0][1],std::max(v[1][1], v[2][1]));

        if(minAabb[0] > 0)
            minAabb[0] -= 1;
        if(minAabb[1] > 0)
            minAabb[1] -= 1;
        if(maxAabb[0] < width_ - 1)
            maxAabb[0] += 1;
        if(maxAabb[1] < height_ - 1)
            maxAabb[1] += 1;

        std::vector<Eigen::Vector2f> pos;

        if(msaa_) {
            msaa_count_ = 1 << msaa_level_;
            float step = 1.0 / (msaa_count_ * 2);
            //std::cout << "msaa_count: " << msaa_count_ << std::endl;
            for(int i = 0; i < msaa_count_; ++i) {
                for(int j = 0; j < msaa_count_; ++j) {
                    pos.push_back({step + i * step * 2, step + j * step * 2});
                }
            }
        }
        else {
            pos.push_back({0.5, 0.5});
        }

        for(int x = minAabb[0]; x < maxAabb[0]; ++x) {
            for(int y = minAabb[1]; y < maxAabb[1]; ++y) {
                // Color Result
                Eigen::Vector3f color_result(0, 0, 0);
                // store min depth
                float minDepth = FLT_MAX;
                // color sample point count
                int count = 0;
                // check sample point
                for (int i = 0; i < pos.size(); i++) {
                    // check point in triangle
                    if (InsideTriangle((float)x + pos[i][0], (float)y + pos[i][1], t.v)) {
                        // Interpolate Z Buffer
                        auto [alpha, beta, gamma] = ComputeBarycentric2D((float)x + pos[i][0], (float)y + pos[i][1], t.v);
                        float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                        float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                        z_interpolated *= w_reciprocal;
                        z_interpolated *= -1;   // invert z
                        minDepth = std::min(minDepth, z_interpolated);
                        color_result += t.GetColor();
                        count++;
                    }
                }
                if (count != 0) {
                    int32_t index = GetIndex(x, y);
                    if(index >= width_ * height_)
                        continue;
                    if (depth_buf_[current_frame_][index] > minDepth) {
                        Eigen::Vector3f color = color_result / (float)pos.size(); //t.GetColor() * count / (float)pos.size();
                        Eigen::Vector2i point(x, y);
                        // Update Depth
                        depth_buf_[current_frame_][index] = minDepth;
                        // Set Pixel Color
                        SetPixel(point, color);
                    }
                }
            }
        }
    }

    void SoftRasterizer::RasterizeTriangleWithShader(const SoftTriangle& t, const std::array<Eigen::Vector3f, 3>& view_pos) {
        auto v = t.ToVector4();
    
        // Find out the bounding box of current triangle.
        // iterate through the pixel and find if the current pixel is inside the triangle
        Eigen::Vector2i minAabb; // left bottom
        Eigen::Vector2i maxAabb; // right up

        minAabb[0] = std::min(v[0][0],std::min(v[1][0], v[2][0]));
        maxAabb[0] = std::max(v[0][0],std::max(v[1][0], v[2][0]));
        minAabb[1] = std::min(v[0][1],std::min(v[1][1], v[2][1]));
        maxAabb[1] = std::max(v[0][1],std::max(v[1][1], v[2][1]));

        if(minAabb[0] > 0)
            minAabb[0] -= 1;
        if(minAabb[1] > 0)
            minAabb[1] -= 1;
        if(maxAabb[0] < width_ - 1)
            maxAabb[0] += 1;
        if(maxAabb[1] < height_ - 1)
            maxAabb[1] += 1;

        std::vector<Eigen::Vector2f> pos;

        if(msaa_) {
            msaa_count_ = 1 << msaa_level_;
            float step = 1.0 / (msaa_count_ * 2);
            //std::cout << "msaa_count: " << msaa_count_ << std::endl;
            for(int i = 0; i < msaa_count_; ++i) {
                for(int j = 0; j < msaa_count_; ++j) {
                    pos.push_back({step + i * step * 2, step + j * step * 2});
                }
            }
        }
        else {
            pos.push_back({0.5, 0.5});
        }

        for(int x = minAabb[0]; x < maxAabb[0]; ++x) {
            for(int y = minAabb[1]; y < maxAabb[1]; ++y) {
                // Color Result
                Eigen::Vector3f color_result(0, 0, 0);
                // store min depth
                float minDepth = FLT_MAX;
                // color sample point count
                int count = 0;
                // check sample point
                for (int i = 0; i < pos.size(); i++) {
                    // check point in triangle
                    if (InsideTriangle((float)x + pos[i][0], (float)y + pos[i][1], t.v)) {
                        // Interpolate Z Buffer
                        auto [alpha, beta, gamma] = ComputeBarycentric2D((float)x + pos[i][0], (float)y + pos[i][1], t.v);
                        float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                        float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                        z_interpolated *= w_reciprocal;
                        z_interpolated *= -1;   // invert z
                        minDepth = std::min(minDepth, z_interpolated);
                        count++;

                        float Z = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                        float zp = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                        zp *= Z;
                        minDepth = std::min(zp, minDepth);

                        auto interpolated_color = Interpolate<Eigen::Vector3f>(Eigen::Vector3f(alpha, beta, gamma), {t.color[0], t.color[1], t.color[2]});
                        auto interpolated_normal = Interpolate<Eigen::Vector3f>(Eigen::Vector3f(alpha, beta, gamma), {t.normal[0], t.normal[1], t.normal[2]});
                        auto interpolated_texcoords = Interpolate<Eigen::Vector2f>(Eigen::Vector3f(alpha, beta, gamma), {t.tex_coords[0], t.tex_coords[1], t.tex_coords[2]});
                        auto interpolated_shadingcoords = Interpolate<Eigen::Vector3f>(Eigen::Vector3f(alpha, beta, gamma), view_pos);

                        auto payload = FragmentShaderPayload(interpolated_color, interpolated_normal.normalized(), interpolated_texcoords, texture ? texture : nullptr);
                        payload.view_pos = interpolated_shadingcoords;
                        auto pixel_color = fragment_shader_(payload);
                        color_result += pixel_color;

                        count++;
                    }
                }
                if (count != 0) {
                    int32_t index = GetIndex(x, y);
                    if(index >= width_ * height_)
                        continue;
                    if (depth_buf_[current_frame_][index] > minDepth) {
                        Eigen::Vector3f color = color_result / (float)pos.size(); //t.GetColor() * count / (float)pos.size();
                        Eigen::Vector2i point(x, y);
                        // Update Depth
                        depth_buf_[current_frame_][index] = minDepth;
                        // Set Pixel Color
                        SetPixel(point, color);
                    }
                }
            }
        }
    }

    void SoftRasterizer::Clear(BufferType buff) {
        if ((buff & BufferType::COLOR) == BufferType::COLOR) {
            std::fill(frame_buf_[current_frame_].begin(), frame_buf_[current_frame_].end(), Eigen::Vector3f{0, 0, 0});
        }
        if ((buff & BufferType::DEPTH) == BufferType::DEPTH) {
            std::fill(depth_buf_[current_frame_].begin(), depth_buf_[current_frame_].end(), std::numeric_limits<float>::infinity());
        }
    }

    void SoftRasterizer::ClearAll(BufferType buff) {
        if ((buff & BufferType::COLOR) == BufferType::COLOR) {
            for(int i = 0; i < FRAME_COUNT; ++i) {
                std::fill(frame_buf_[i].begin(), frame_buf_[i].end(), Eigen::Vector3f{0, 0, 0});
            }
        }
        if ((buff & BufferType::DEPTH) == BufferType::DEPTH) {
            for(int i = 0; i < FRAME_COUNT; ++i) {
                std::fill(depth_buf_[i].begin(), depth_buf_[i].end(), std::numeric_limits<float>::infinity());
            }
        }
    }

    float SoftRasterizer::FindDepth(int32_t x, int32_t y) {
        int32_t index = GetIndex(x, y);
        if(index >= width_ * height_)
            return FLT_MAX;
        else 
            return depth_buf_[current_frame_][index];
    }

    void SoftRasterizer::SetPixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color) {
        //old index: auto ind = point.y() + point.x() * width;
        if (point[0] < 0 || point[0] >= width_ || point[1] < 0 || point[1] >= height_)
            return;
        auto ind = (height_ - 1 - point[1]) * width_ + point[0];
        frame_buf_[current_frame_][ind] = color / 255.0;
    }

    void SoftRasterizer::SetPixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color) {
        if (point[0] < 0 || point[0] >= width_ || point[1] < 0 || point[1] >= height_)
            return;
        auto ind = (height_ - 1 - point[1]) * width_ + point[0];
        frame_buf_[current_frame_][ind] = color / 255.0;
    }
}
