#pragma once
#include "Math/Vectors.h"
#include "Algorism/StringOperation.h"

#include <string>
#include <string_view>

namespace Rocket {
    // TODO : combine this into Geometry::Vertex
    struct Vertex {
        // Position Vector
        Vector3f position;
        // Normal Vector
        Vector3f normal;
        // Texture Coordinate Vector
        Vector2f texture_coordinate;
    };

    // TODO : combine this into Geometry::Material
    struct Material {
        Material() {
            name = "";
            ns = 0.0f;
            ni = 0.0f;
            d = 0.0f;
            illum = 0;
        }

        // Material Name
        std::string name;
        // Ambient Color
        Vector3f ka;
        // Diffuse Color
        Vector3f kd;
        // Specular Color
        Vector3f ks;
        // Specular Exponent
        float ns;
        // Optical Density
        float ni;
        // Dissolve
        float d;
        // Illumination
        int illum;
        // Ambient Texture Map
        std::string map_ka;
        // Diffuse Texture Map
        std::string map_kd;
        // Specular Texture Map
        std::string map_ks;
        // Specular Hightlight Map
        std::string map_ns;
        // Alpha Texture Map
        std::string map_d;
        // Bump Map
        std::string map_bump;
    };

    // TODO : combine this into Geometry::Mesh
    struct Mesh {
        Mesh() {}
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
        ObjParser(const std::string& path, const std::string& name) : path_(path), name_(name) {}
        virtual ~ObjParser() = default;

        int32_t Initialize();
        void Finalize();
        void Parse();

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
    };
}
