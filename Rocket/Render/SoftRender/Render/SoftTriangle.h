#pragma once
#include "Geometry/Triangle.h"
#include "Render/SoftTexture.h"

#include <Eigen/Eigen>

#include <cstdint>
#include <array>
#include <memory>

namespace Rocket {
    class SoftTriangle {
    public:
        Eigen::Vector4f v[3]; // the original coordinates of the triangle, v0, v1, v2 in counter clockwise order
        // Per vertex values
        Eigen::Vector3f color[3];      // color at each vertex;
        Eigen::Vector2f tex_coords[3]; // texture u,v
        Eigen::Vector3f normal[3];     // normal vector for each vertex

        SoftTexturePtr texture = nullptr;

    public:
        SoftTriangle();
        SoftTriangle(Geometry::TrianglePtr tri);
        virtual ~SoftTriangle() = default;

        Eigen::Vector3f a3f() const { return v[0].head<3>(); }
        Eigen::Vector3f b3f() const { return v[1].head<3>(); }
        Eigen::Vector3f c3f() const { return v[2].head<3>(); }
        Eigen::Vector4f a4f() const { return v[0]; }
        Eigen::Vector4f b4f() const { return v[1]; }
        Eigen::Vector4f c4f() const { return v[2]; }

        Eigen::Vector3f GetColor(int32_t index = 0) const { return color[index] * 255; }

        void SetVertex(int32_t ind, const Eigen::Vector4f& ver) { v[ind] = ver; } // set i-th vertex coordinates 
        void SetVertex(int32_t ind, const Eigen::Vector3f& ver) { v[ind] = Eigen::Vector4f(ver[0], ver[1], ver[2], 1.0); } // set i-th vertex coordinates 
        void SetNormal(int32_t ind, const Eigen::Vector3f& n) { normal[ind] = n; } // set i-th vertex normal vector
        void SetTexCoord(int32_t ind, float s, float t) { tex_coords[ind] = Eigen::Vector2f(s, t); } // set i-th vertex texture coordinate
        void SetTexCoord(int32_t ind, const Eigen::Vector2f& tex) { tex_coords[ind] = tex; }
        void SetColor(int32_t ind, float r, float g, float b); // set i-th vertex color
        
        void SetNormals(const std::array<Eigen::Vector3f, 3>& normals);
        void SetColors(const std::array<Eigen::Vector3f, 3>& colors);

        std::array<Eigen::Vector3f, 3> ToVector3() const;
        std::array<Eigen::Vector4f, 3> ToVector4() const;
    };

    using SoftTrianglePtr = std::shared_ptr<SoftTriangle>;
}
