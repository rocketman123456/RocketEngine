#include "Render/SoftRasterizer.h"

namespace Rocket {
    static auto to_vec4(const Eigen::Vector3f &v3, float w = 1.0f) {
        return Eigen::Vector4f(v3.x(), v3.y(), v3.z(), w);
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

    std::tuple<float, float, float> SoftRasterizer::ComputeBarycentric2D(float x, float y, const Eigen::Vector3f* v) {
        float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
        float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
        float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
        return {c1,c2,c3};
    }

    // Bresenham's line drawing algorithm
    // Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
    void SoftRasterizer::DrawLine(Eigen::Vector3f begin, Eigen::Vector3f end) {
        auto x1 = begin.x();
        auto y1 = begin.y();
        auto x2 = end.x();
        auto y2 = end.y();

        Eigen::Vector3f line_color = {255, 255, 255};

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
            }
            else {
                x = x2;
                y = y2;
                xe = x1;
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
                //            delay(0);
                Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
                SetPixel(point, line_color);
            }
        }
        else {
            if (dy >= 0) {
                x = x1;
                y = y1;
                ye = y2;
            }
            else {
                x = x2;
                y = y2;
                ye = y1;
            }
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
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
                //            delay(0);
                Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
                SetPixel(point, line_color);
            }
        }
    }

    void SoftRasterizer::NextFrame() {
        last_frame_ = current_frame_;
        current_frame_++;
        current_frame_ = current_frame_ % FRAME_COUNT;
    }

    void SoftRasterizer::Draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, RenderPrimitive type) {
        if (type != RenderPrimitive::TRIANGLE) {
            throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
        }

        auto &buf = pos_buf_[pos_buffer.pos_id];
        auto &ind = ind_buf_[ind_buffer.ind_id];

        float f1 = (100 - 0.1) / 2.0;
        float f2 = (100 + 0.1) / 2.0;

        Eigen::Matrix4f mvp = projection_ * view_ * model_;
        for (auto &i : ind) {
            SoftTriangle t;

            Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
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
                t.SetVertex(i, v[i].head<3>());
                // t.SetVertex(i, v[i].head<3>());
                // t.SetVertex(i, v[i].head<3>());
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

        Eigen::Matrix4f mvp = projection_ * view_ * model_;
        for (auto& i : ind)
        {
            SoftTriangle t;
            Eigen::Vector4f v[] = {
                    mvp * to_vec4(buf[i[0]], 1.0f),
                    mvp * to_vec4(buf[i[1]], 1.0f),
                    mvp * to_vec4(buf[i[2]], 1.0f)
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
                t.SetVertex(i, v[i].head<3>());
            }

            auto col_x = col[i[0]];
            auto col_y = col[i[1]];
            auto col_z = col[i[2]];

            // TODO : add color interpolate
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
        DrawLine(t.c(), t.a());
        DrawLine(t.c(), t.b());
        DrawLine(t.b(), t.a());
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
            pos.push_back({0.25,0.25});
            pos.push_back({0.75,0.25});
            pos.push_back({0.25,0.75});
            pos.push_back({0.75,0.75});
        }
        else {
            pos.push_back({0.5, 0.5});
        }

        for(int x = minAabb[0]; x < maxAabb[0]; ++x) {
            for(int y = minAabb[1]; y < maxAabb[1]; ++y) {
                // store min depth
                float minDepth = FLT_MAX;
                // color sample point count
                int count = 0;
                // check four sample point
                for (int i = 0; i < pos.size(); i++) {
                    // check point in triangle
                    if (InsideTriangle((float)x + pos[i][0], (float)y + pos[i][1], t.v)) {
                        // Interpolate Z Buffer
                        auto [alpha, beta, gamma] = ComputeBarycentric2D((float)x + pos[i][0], (float)y + pos[i][1], t.v);
                        float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                        float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                        z_interpolated *= w_reciprocal;
                        minDepth = std::min(minDepth, z_interpolated);
                        count++;
                    }
                }
                if (count != 0) {
                    if (depth_buf_[current_frame_][GetIndex(x, y)] > minDepth) {
                        Eigen::Vector3f color = t.GetColor() * count / (float)pos.size();
                        Eigen::Vector3f point(3);
                        point << (float)x, (float)y, minDepth;
                        // Update Depth
                        depth_buf_[current_frame_][GetIndex(x, y)] = minDepth;
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

    SoftRasterizer::SoftRasterizer(int w, int h) : width_(w), height_(h) {
        for(int i = 0; i < FRAME_COUNT; ++i) {
            frame_buf_[i].resize(w * h);
            depth_buf_[i].resize(w * h);
        }
    }

    void SoftRasterizer::SetPixel(const Eigen::Vector3f &point, const Eigen::Vector3f &color) {
        //old index: auto ind = point.y() + point.x() * width;
        if (point.x() < 0 || point.x() >= width_ || point.y() < 0 || point.y() >= height_)
            return;
        auto ind = (height_ - 1 - point.y()) * width_ + point.x();
        frame_buf_[current_frame_][ind] = color;
    }
}
