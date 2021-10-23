#include "Render/SoftRasterizer.h"
#include "Math/Parameter.h"
#include "Math/ComonMath.h"
#include "Math/Vectors.h"
#include "Log/Log.h"

#include <cfloat>
#include <iostream>

namespace Rocket {
    SoftRasterizer::SoftRasterizer(int w, int h) : width_(w), height_(h) {
        // Resize to Proper Size
        for(int i = 0; i < FRAME_COUNT; ++i) {
            frame_buf_[i].resize(w * h);
            depth_buf_[i].resize(w * h);
        }
        msaa_pos.clear();
        msaa_pos.push_back({0.5, 0.5});
    }

    pos_buf_id SoftRasterizer::LoadPositions(const Vector3fVec& positions) {
        auto id = GetNextId();
        pos_buf_.emplace(id, positions);
        return {id};
    }

    ind_buf_id SoftRasterizer::LoadIndices(const Vector3iVec& indices) {
        auto id = GetNextId();
        ind_buf_.emplace(id, indices);
        return {id};
    }

    col_buf_id SoftRasterizer::LoadColors(const Vector3fVec& colors) {
        auto id = GetNextId();
        col_buf_.emplace(id, colors);
        return {id};
    }

    col_buf_id SoftRasterizer::LoadNormals(const Vector3fVec& normals) {
        auto id = GetNextId();
        nor_buf_.emplace(id, normals);
        normal_id_ = id;
        return {id};
    }

    bool SoftRasterizer::InsideTriangle(float x, float y, const Eigen::Vector4f _v[]) {
        Eigen::Vector3f v[3];
        for(int i=0;i<3;i++)
            v[i] = {_v[i].x(),_v[i].y(), 1.0};
        Eigen::Vector3f f0,f1,f2;
        f0 = v[1].cross(v[0]);
        f1 = v[2].cross(v[1]);
        f2 = v[0].cross(v[2]);
        Eigen::Vector3f p(x,y,1.);
        if((p.dot(f0)*f0.dot(v[2])>0) && (p.dot(f1)*f1.dot(v[0])>0) && (p.dot(f2)*f2.dot(v[1])>0))
            return true;
        else if((p.dot(f0)*f0.dot(v[2])<0) && (p.dot(f1)*f1.dot(v[0])<0) && (p.dot(f2)*f2.dot(v[1])<0))
            return true;
        return false;

        // Vector2f screen_point = Vector2f(x, y);
        // Vector2f v[3];
        // v[0] = _v[1].head(2) - _v[0].head(2);
        // v[1] = _v[2].head(2) - _v[1].head(2);
        // v[2] = _v[0].head(2) - _v[2].head(2);
        // Vector2f dir[3];
        // dir[0] = screen_point - _v[0].head(2);
        // dir[1] = screen_point - _v[1].head(2);
        // dir[2] = screen_point - _v[2].head(2);
        // int count = 0;
        // for(int i = 0; i < 3; ++i) {
        //     float result = v[i][0] * dir[i][1] - v[i][1] * dir[i][0];
        //     if(result > 0) {
        //         count++;
        //     } else if(result < 0) {
        //         count--;
        //     }
        // }
        // return count == 3 || count == -3;
    }

    std::tuple<float, float, float> SoftRasterizer::ComputeBarycentric2D(float x, float y, const Eigen::Vector4f v[]) {
        float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / 
            (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
        float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / 
            (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
        float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / 
            (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
        return { c1, c2, c3 };
    }

    void SoftRasterizer::SetPixel(const Eigen::Vector2i& point, const Eigen::Vector3f& color) {
        if (point[0] < 0 || point[0] >= width_ || point[1] < 0 || point[1] >= height_)
            return;
        auto ind = (height_ - point[1]) * width_ + point[0];
        // TODO : Check whether to use this clamp function
        auto color_ = color;
        for(int i = 0; i < 3; ++i) {
            float data = color_[i];
            data = data > 255.0f-EPS ? 255.0f : data;
            data = data < 0.0f+EPS ? 0.0f : data;
            color_[i] = data;
        }
        frame_buf_[current_frame_][ind] = color_ / 255.0;
    }

    float SoftRasterizer::GetDepth(int32_t x, int32_t y) {
        if (x < 0 || x >= width_ || y < 0 || y >= height_)
            return FLT_MAX;
        auto ind = (height_ - y) * width_ + x;
        return depth_buf_[current_frame_][ind];
    }

    void SoftRasterizer::SetDepth(const Eigen::Vector2i& point, const double depth) {
        if (point[0] < 0 || point[0] >= width_ || point[1] < 0 || point[1] >= height_)
            return;
        auto ind = (height_ - point[1]) * width_ + point[0];
        if(depth_buf_[current_frame_][ind] > depth) {
            depth_buf_[current_frame_][ind] = depth;
        }
    }

    void SoftRasterizer::DrawLine3D(const Eigen::Vector3f& begin, const Eigen::Vector3f& end) {
        DrawLine3D(begin, end, {255, 255, 255}, {255, 255, 255});
    }

    void SoftRasterizer::DrawLines3D(const Vector3fVec& begin, const Vector3fVec& end) {
        assert(begin.size() == end.size());
        for(int32_t i = 0; i < begin.size(); ++i) {
            DrawLine3D(begin[i], end[i], {255, 255, 255}, {255, 255, 255});
        }
    }

    void SoftRasterizer::DrawPoint3D(const Eigen::Vector3f& point) {
        Eigen::Vector3f point_color = {255, 255, 255};
        DrawLine3D(point, point, point_color, point_color);
    }

    void SoftRasterizer::DrawPoint3D(const Eigen::Vector3f& point, const Eigen::Vector3f& color) {
        DrawLine3D(point, point, color, color);
    }

    void SoftRasterizer::DrawPoints3D(const Vector3fVec& point) {
        Eigen::Vector3f point_color = {255, 255, 255};
        for(auto single_point : point) {
            DrawPoint3D(single_point, point_color);
        }
    }

    void SoftRasterizer::DrawPoints3D(const std::vector<Eigen::Vector3f>& point, const std::vector<Eigen::Vector3f>& color) {
        assert(point.size() == color.size());
        
        for(int32_t i = 0; i < point.size(); i++) {
            DrawPoint3D(point[i], color[i]);
        }
    }

    void SoftRasterizer::DrawLines3D(
        const Vector3fVec& begin, const Vector3fVec& end, 
        const Vector3fVec& color_begin, const Vector3fVec& color_end) {

        assert(begin.size() == end.size());
        assert(color_begin.size() == color_end.size());
        assert(begin.size() == color_begin.size());

        for(int32_t i = 0; i < begin.size(); ++i) {
            DrawLine3D(begin[i], end[i], color_begin[i], color_end[i]);
        }
    }

    void SoftRasterizer::DrawLine3D(
        const Eigen::Vector3f& begin, const Eigen::Vector3f& end, 
        const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end) {

        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        Eigen::Vector4f v[] = {
            mvp_ * Math::to_vec4(begin, 1.0f),
            mvp_ * Math::to_vec4(end, 1.0f),
        };

        // Homogeneous division
        for (auto &vec : v) {
            vec /= vec.w();
            vec.w() = 1.0f;
        }

        // Viewport transformation
        for (auto &vert : v) {
            vert.x() = 0.5 * width_ * (vert.x() + 1.0);
            vert.y() = 0.5 * height_ * (vert.y() + 1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        DrawLine(v[0].head<3>(), v[1].head<3>(), color_begin, color_end);
    }

    // Bresenham's line drawing algorithm
    // Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
    void SoftRasterizer::DrawLine(
        const Eigen::Vector3f& begin, const Eigen::Vector3f& end, 
        const Eigen::Vector3f& color_begin, const Eigen::Vector3f& color_end) {
        
        auto x1 = begin.x();
        auto y1 = begin.y();
        auto z1 = begin.z();
        auto x2 = end.x();
        auto y2 = end.y();
        auto z2 = end.z();

        // For Color Interpolate
        Eigen::Vector3f color_delta = Eigen::Vector3f::Zero();
        Eigen::Vector3f line_color = Eigen::Vector3f::Zero();
        float length = (end - begin).norm();

        // For Depth Interpolate
        int z, dz, dz1, pz, ze;

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
                z = z1;
                xe = x2;
                line_color = color_begin;
                color_delta = color_end - color_begin;
                z = z1;
                dz = z2 - z1;
            } else {
                x = x2;
                y = y2;
                xe = x1;
                line_color = color_end;
                color_delta = color_begin - color_end;
                z = z2;
                dz = z1 - z2;
            }
            if(length > EPS) {
                color_delta = color_delta / length;
                dz = dz / length;
            }
            Eigen::Vector2i point = Eigen::Vector2i(x, y);
            SetPixel(point, line_color);
            SetDepth(point, z);
            for (i = 0; x < xe; i++) {
                x = x + 1;
                if (px < 0) {
                    px = px + 2 * dy1;
                } else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        y = y + 1;
                    } else {
                        y = y - 1;
                    }
                    px = px + 2 * (dy1 - dx1);
                }
                Eigen::Vector2i point(x, y);
                line_color += color_delta;
                SetPixel(point, line_color);
                z += dz;
                SetDepth(point, z);
            }
        }
        else {
            if (dy >= 0) {
                x = x1;
                y = y1;
                ye = y2;
                line_color = color_begin;
                color_delta = color_end - color_begin;
                z = z1;
                dz = z2 - z1;
            } else {
                x = x2;
                y = y2;
                ye = y1;
                line_color = color_end;
                color_delta = color_begin - color_end;
                z = z2;
                dz = z1 - z2;
            }
            if(length > EPS) {
                color_delta = color_delta / length;
                dz = dz / length;
            }
            Eigen::Vector2i point(x, y);
            SetPixel(point, line_color);
            SetDepth(point, z);
            for (i = 0; y < ye; i++) {
                y = y + 1;
                if (py <= 0) {
                    py = py + 2 * dx1;
                } else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                        x = x + 1;
                    } else {
                        x = x - 1;
                    }
                    py = py + 2 * (dx1 - dy1);
                }
                Eigen::Vector2i point(x, y);
                line_color += color_delta;
                SetPixel(point, line_color);
                z += dz;
                SetDepth(point, z);
            }
        }
    }

    void SoftRasterizer::Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, RenderPrimitive type) {
        if (type != RenderPrimitive::TRIANGLE) {
            throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
        }

        auto &buf = pos_buf_[pos_buffer.pos_id];
        auto &ind = ind_buf_[ind_buffer.ind_id];

        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        for (auto &i : ind) {
            SoftTriangle t;

            Eigen::Vector4f v[] = {
                mvp_ * Math::to_vec4(buf[i[0]], 1.0f),
                mvp_ * Math::to_vec4(buf[i[1]], 1.0f),
                mvp_ * Math::to_vec4(buf[i[2]], 1.0f)
            };

            // Homogeneous division
            for (auto &vec : v) {
                vec /= vec.w();
                vec.w() = 1.0f;
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

            if(wireframe_)
                RasterizeWireframe(t);
            else
                RasterizeTriangle(t);
        }
    }

    void SoftRasterizer::Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, RenderPrimitive type) {
        if (type != RenderPrimitive::TRIANGLE) {
            throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
        }

        auto& buf = pos_buf_[pos_buffer.pos_id];
        auto& ind = ind_buf_[ind_buffer.ind_id];
        auto& col = col_buf_[col_buffer.col_id];

        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        for (auto& i : ind) {
            SoftTriangle t;
            Eigen::Vector4f v[] = {
                mvp_ * Math::to_vec4(buf[i[0]], 1.0f),
                mvp_ * Math::to_vec4(buf[i[1]], 1.0f),
                mvp_ * Math::to_vec4(buf[i[2]], 1.0f)
            };
            // Homogeneous division
            for (auto &vec : v) {
                vec /= vec.w();
                vec.w() = 1.0f;
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

    void SoftRasterizer::Draw(const std::vector<SoftTrianglePtr>& TriangleList) {
        int32_t count = 0;
        for (auto& t : TriangleList) {
            RK_TRACE(Render, "Render {} Triangle", count++);
            Draw(t);
        }
    }

    void SoftRasterizer::Draw(const SoftTrianglePtr& t) {
        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        SoftTriangle newtri = *t;

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
            vec /= vec.w();
            vec.w() = 1.0f;
        }

        Eigen::Matrix4f inv_trans = (view_ * model_).inverse().transpose();
        Eigen::Vector4f n[] = {
            inv_trans * Math::to_vec4(t->normal[0], 0.0f),
            inv_trans * Math::to_vec4(t->normal[1], 0.0f),
            inv_trans * Math::to_vec4(t->normal[2], 0.0f)
        };

        //Viewport transformation
        for (auto & vert : v) {
            vert.x() = 0.5 * width_ * (vert.x() + 1.0);
            vert.y() = 0.5 * height_ * (vert.y() + 1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i) {
            //screen space coordinates
            newtri.SetVertex(i, v[i]);
        }

        for (int i = 0; i < 3; ++i) {
            //view space normal
            newtri.SetNormal(i, n[i].head<3>());
        }

        newtri.SetColor(0, 148,121.0,92.0);
        newtri.SetColor(1, 148,121.0,92.0);
        newtri.SetColor(2, 148,121.0,92.0);

        // Also pass view space vertice position
        if(wireframe_)
            RasterizeWireframe(newtri);
        else
            RasterizeTriangle(newtri, viewspace_pos);
    }

    void SoftRasterizer::RasterizeWireframe(const SoftTriangle& t) {
        Eigen::Vector3f color_begin = { 255, 255, 255 };
        Eigen::Vector3f color_end = { 0, 255, 0 };
        DrawLine(t.c3f(), t.a3f(), color_begin, color_end);
        DrawLine(t.c3f(), t.b3f(), color_begin, color_end);
        DrawLine(t.b3f(), t.a3f(), color_begin, color_end);
    }

    void SoftRasterizer::RasterizeTriangle(const SoftTriangle& t, const std::array<Eigen::Vector3f, 3>& view_pos) {
        auto v = t.ToVector4();
    
        // Find out the bounding box of current triangle.
        // iterate through the pixel and find if the current pixel is inside the triangle
        Eigen::Vector2f minAabbf; // left bottom
        Eigen::Vector2f maxAabbf; // right up

        minAabbf[0] = std::min(v[0][0],std::min(v[1][0], v[2][0]));
        maxAabbf[0] = std::max(v[0][0],std::max(v[1][0], v[2][0]));
        minAabbf[1] = std::min(v[0][1],std::min(v[1][1], v[2][1]));
        maxAabbf[1] = std::max(v[0][1],std::max(v[1][1], v[2][1]));

        Eigen::Vector2i minAabb;
        Eigen::Vector2i maxAabb;

        minAabb[0] = std::floor(minAabbf[0]);
        maxAabb[0] = std::ceil(maxAabbf[0]);
        minAabb[1] = std::floor(minAabbf[1]);
        maxAabb[1] = std::ceil(maxAabbf[1]);

        for(int x = minAabb[0]; x < maxAabb[0]; ++x) {
            for(int y = minAabb[1]; y < maxAabb[1]; ++y) {
                // store min depth
                float min_depth = FLT_MAX;
                // color sample point count
                int count = 0;
                // check sample point
                for (int i = 0; i < msaa_pos.size(); i++) {
                    // check point in triangle
                    float fx = (float)x + msaa_pos[i][0];
                    float fy = (float)y + msaa_pos[i][1];
                    bool inside = InsideTriangle(fx, fy, t.v);
                    if(inside) {
                        // Interpolate Z Buffer
                        auto[alpha, beta, gamma] = ComputeBarycentric2D((float)x + msaa_pos[i][0], (float)y + msaa_pos[i][1], t.v);
                        float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                        float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                        z_interpolated *= w_reciprocal;
                        z_interpolated *= -1;
                        min_depth = std::min(min_depth, z_interpolated);
                        count++;
                    }
                }
                if(count != 0) {
                    // if(GetIndex(x, y) >= width_ * height_)
                    //     continue;
                    if(GetDepth(x, y) > min_depth) {
                        float fx = (float)x + 0.5;
                        float fy = (float)y + 0.5;
                        // Color Result
                        Eigen::Vector3f color_result(0, 0, 0);
                        auto [alpha, beta, gamma] = ComputeBarycentric2D(fx, fy, t.v);
                        if(with_shader_) {
                            auto interpolated_color = Interpolate<Eigen::Vector3f>(Eigen::Vector3f(alpha, beta, gamma), {t.color[0], t.color[1], t.color[2]});
                            auto interpolated_normal = Interpolate<Eigen::Vector3f>(Eigen::Vector3f(alpha, beta, gamma), {t.normal[0], t.normal[1], t.normal[2]});
                            auto interpolated_texcoords = Interpolate<Eigen::Vector2f>(Eigen::Vector3f(alpha, beta, gamma), {t.tex_coords[0], t.tex_coords[1], t.tex_coords[2]});
                            auto interpolated_shadingcoords = Interpolate<Eigen::Vector3f>(Eigen::Vector3f(alpha, beta, gamma), view_pos);

                            auto payload = FragmentShaderPayload(
                                interpolated_color, 
                                interpolated_normal.normalized(), 
                                interpolated_texcoords, 
                                texture_ ? texture_.value() : nullptr);
                            payload.view_pos = interpolated_shadingcoords;
                            color_result = fragment_shader_(payload);
                        } else {
                            color_result = t.GetColor();
                        }
                        Eigen::Vector3f color = color_result * (float)count / (float)msaa_pos.size();
                        Eigen::Vector2i point(x, y);
                        // Update Depth
                        SetDepth(point, min_depth);
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
}
