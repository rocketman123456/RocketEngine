#pragma once

#include <Eigen/Eigen>

#include <cstdint>
#include <array>

namespace Rocket {
    class SoftTriangle {
    public:
        Eigen::Vector3f v[3]; // the original coordinates of the triangle, v0, v1, v2 in counter clockwise order
        // Per vertex values
        Eigen::Vector3f color[3];      // color at each vertex;
        Eigen::Vector2f tex_coords[3]; // texture u,v
        Eigen::Vector3f normal[3];     // normal vector for each vertex

        // Texture *tex;
        SoftTriangle();

        Eigen::Vector3f a() const { return v[0]; }
        Eigen::Vector3f b() const { return v[1]; }
        Eigen::Vector3f c() const { return v[2]; }

        inline void SetVertex(int32_t ind, Eigen::Vector3f ver) { v[ind] = ver; } // set i-th vertex coordinates 
        inline void SetNormal(int32_t ind, Eigen::Vector3f n) { normal[ind] = n; } // set i-th vertex normal vector
        void SetColor(int32_t ind, float r, float g, float b); // set i-th vertex color
        Eigen::Vector3f GetColor(int32_t index = 0) const { return color[index] * 255; }
        void SetTexCoord(int32_t ind, float s, float t); // set i-th vertex texture coordinate
        std::array<Eigen::Vector4f, 3> ToVector4() const;
    };
}
