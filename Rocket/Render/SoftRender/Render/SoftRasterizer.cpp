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
                mvp * to_vec4(buf[i[2]], 1.0f)};

            for (auto &vec : v) {
                vec /= vec.w();
            }

            for (auto &vert : v) {
                vert.x() = 0.5 * width_ * (vert.x() + 1.0);
                vert.y() = 0.5 * height_ * (vert.y() + 1.0);
                vert.z() = vert.z() * f1 + f2;
            }

            for (int i = 0; i < 3; ++i) {
                t.SetVertex(i, v[i].head<3>());
                t.SetVertex(i, v[i].head<3>());
                t.SetVertex(i, v[i].head<3>());
            }

            t.SetColor(0, 255.0, 0.0, 0.0);
            t.SetColor(1, 0.0, 255.0, 0.0);
            t.SetColor(2, 0.0, 0.0, 255.0);

            RasterizeWireframe(t);
        }
    }

    void SoftRasterizer::RasterizeWireframe(const SoftTriangle &t) {
        DrawLine(t.c(), t.a());
        DrawLine(t.c(), t.b());
        DrawLine(t.b(), t.a());
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
        auto ind = (height_ - point.y()) * width_ + point.x();
        frame_buf_[current_frame_][ind] = color;
    }
}
