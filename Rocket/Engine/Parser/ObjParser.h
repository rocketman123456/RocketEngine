#pragma once
#include "Math/Vectors.h"
#include "Algorithm/StringOperation.h"

#include <string>
#include <string_view>

namespace Rocket {
    // TODO : combine this into Geometry::Vertex
    struct Vertex {
        // Position Vector
        Eigen::Vector3f position = Eigen::Vector3f::Zero();
        // Normal Vector
        Eigen::Vector3f normal = Eigen::Vector3f::Zero();
        // Texture Coordinate Vector
        Eigen::Vector2f texture_coordinate = Eigen::Vector2f::Zero();
    };

    // TODO : combine this into Geometry::Material
    struct Material {
        // Material Name
        std::string name = "";
        // Ambient Color
        Eigen::Vector3f ka = Eigen::Vector3f::Zero();
        // Diffuse Color
        Eigen::Vector3f kd = Eigen::Vector3f::Zero();
        // Specular Color
        Eigen::Vector3f ks = Eigen::Vector3f::Zero();
        // Specular Exponent
        float ns = 0.0f;
        // Optical Density
        float ni = 0.0f;
        // Dissolve
        float d = 0.0f;
        // Illumination
        int illum = 0;
        // Ambient Texture Map
        std::string map_ka = "";
        // Diffuse Texture Map
        std::string map_kd = "";
        // Specular Texture Map
        std::string map_ks = "";
        // Specular Hightlight Map
        std::string map_ns = "";
        // Alpha Texture Map
        std::string map_d = "";
        // Bump Map
        std::string map_bump = "";
    };

    // TODO : combine this into Geometry::Mesh
    struct Mesh {
        Mesh() = default;
        // Variable Set Constructor
        Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices) {
            vertices = _vertices;
            indices = _indices;
        }
        // Mesh Name
        std::string mesh_name;
        // Vertex List
        std::vector<Vertex> vertices;
        // Index List
        std::vector<unsigned int> indices;
        // Material
        Material mesh_material;
    };

    class ObjParser {
    public:
        ObjParser(const std::string& path, const std::string& name) 
            : path_(path), name_(name) { full_path_ = path_ + name_; }
        virtual ~ObjParser() = default;

        int32_t Initialize();
        void Finalize();
        bool Parse();

        const std::string& GetContent() const { return content_; }
        const std::string& GetFullPath() const { return full_path_; }

    private:
        void GenVerticesFromRawOBJ(
            std::vector<Vertex>& oVerts,
            const std::vector<Eigen::Vector3f>& iPositions,
            const std::vector<Eigen::Vector2f>& iTCoords,
            const std::vector<Eigen::Vector3f>& iNormals,
            std::string icurline);
        bool LoadMaterials(std::string path);
        void VertexTriangluation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts);

    public:
        // Loaded Mesh Objects
        std::vector<Mesh> loaded_meshes;
        // Loaded Vertex Objects
        std::vector<Vertex> loaded_vertices;
        // Loaded Index Positions
        std::vector<unsigned int> loaded_indices;
        // Loaded Material Objects
        std::vector<Material> loaded_materials;

    private:
        std::string path_;
        std::string name_;
        std::string full_path_;
        std::string content_;
        std::vector<std::string> material_content_;
    };
}
