#include "Log/Log.h"
#include "Window/DesktopWindow.h"
#include "Render/SoftRasterizer.h"
#include "Render/SoftTriangle.h"
#include "Parser/ObjParser.h"
#include "Utils/FindRootDir.h"

#include "Geometry/Sphere.h"

using namespace Rocket;

#include "../Utils/soft_render.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    Log::Init();

    RenderApp app;
    app.Initialize(1280, 720);

    // load image, create texture and generate mipmaps
    int32_t width = 1280;
    int32_t height = 720;
    int32_t nrChannels = 4;

    SoftRasterizer rst(width, height);
    rst.ClearAll(BufferType::COLOR | BufferType::DEPTH);

    Eigen::Vector3f eye_pos = {0.0, 0.0, 5};

    int32_t key = 0;
    int32_t frame_count = 0;

    std::string name = "_root_dir_";
    auto root = FindRootDir(name);
    root += "/";

    //std::string model_path = "Asset/Model/rock/rock.obj";
    //std::string texture_path = "Asset/Model/rock/rock.png";

    std::string model_path = "Asset/Model/spot/spot_triangulated_good.obj";
    std::string texture_path = "Asset/Model/spot/spot_texture.png";

    ObjParser parser(root, model_path);
    parser.Initialize();
    parser.Parse();

    SoftTexturePtr texture = SoftTexturePtr(new SoftTexture(root, texture_path));

    std::vector<SoftTrianglePtr> triangle_list;

    for(auto mesh : parser.loaded_meshes) {
        for(int i=0;i<mesh.vertices.size();i+=3) {
            SoftTrianglePtr t(new SoftTriangle());
            for(int j=0;j<3;j++) {
                t->SetVertex(j, Eigen::Vector4f(mesh.vertices[i+j].position[0], mesh.vertices[i+j].position[1], mesh.vertices[i+j].position[2], 1.0));
                t->SetNormal(j, Eigen::Vector3f(mesh.vertices[i+j].normal[0], mesh.vertices[i+j].normal[1], mesh.vertices[i+j].normal[2]));
                t->SetTexCoord(j, Eigen::Vector2f(mesh.vertices[i+j].texture_coordinate[0], mesh.vertices[i+j].texture_coordinate[1]));
            }
            triangle_list.push_back(t);
        }
    }

    std::cout << "Tirangle Count: " << triangle_list.size() << std::endl;

    VertexShaderFunc vertex_shader = ::vertex_shader;
    FragmentShaderFunc fragment_shader = ::texture_fragment_shader;

    SoftTrianglePtr tri = SoftTrianglePtr(new SoftTriangle());
    tri->SetVertex(0, Eigen::Vector3f(2, 0, -2));
    tri->SetVertex(1, Eigen::Vector3f(0, 2, -2));
    tri->SetVertex(2, Eigen::Vector3f(-2, 0, -2));
    tri->SetColor(0, 217.0, 238.0, 185.0);
    tri->SetColor(1, 217.0, 238.0, 185.0);
    tri->SetColor(2, 217.0, 238.0, 185.0);
    tri->SetNormal(0, Eigen::Vector3f(0, 0, 1));
    tri->SetNormal(1, Eigen::Vector3f(0, 0, 1));
    tri->SetNormal(2, Eigen::Vector3f(0, 0, 1));

    rst.SetTexture(texture);
    rst.SetVertexShader(vertex_shader);
    rst.SetFragmentShader(fragment_shader);
    rst.DisableWireFrame();
    //rst.EnableWireFrame();
    rst.DisableMsaa();
    //rst.EnableMsaa();
    //rst.SetMsaaLevel(0);
    rst.EnableShader();

    int32_t count = 0;
    std::cout << "Initialize Finished" << std::endl;
    
    while(!app.ShouldClose()) {
        app.Tick();

        rst.NextFrame();
        rst.Clear(BufferType::COLOR | BufferType::DEPTH);
        rst.SetModel(get_model_matrix(global_angle_x, global_angle_y, global_angle_z));
        rst.SetView(get_view_matrix(eye_pos));
        rst.SetProjection(get_perspective_matrix(45, ((float)width/(float)height), 0.1, 50));
        //rst.SetProjection(get_orthographic_matrix(-6.4, 6.4, -50, 50, 3.6, -3.6));
        RK_INFO(App, "Set MVP");

        rst.DrawLine3D({0,0,0}, {1,0,0}, {255,0,0}, {255,0,0}); // x
        rst.DrawLine3D({0,0,0}, {0,1,0}, {0,255,0}, {0,255,0}); // y
        rst.DrawLine3D({0,0,0}, {0,0,1}, {0,0,255}, {0,0,255}); // z

        // for(SoftTrianglePtr& face : triangle_list) {
        //     rst.DrawLine3D(
        //         Eigen::Vector3f(face->v[0][0], face->v[0][1], face->v[0][2]), 
        //         Eigen::Vector3f(face->v[1][0], face->v[1][1], face->v[1][2]),
        //         Eigen::Vector3f(255,0,0),
        //         Eigen::Vector3f(0,255,0)
        //     );
        //     rst.DrawLine3D(
        //         Eigen::Vector3f(face->v[1][0], face->v[1][1], face->v[1][2]), 
        //         Eigen::Vector3f(face->v[2][0], face->v[2][1], face->v[2][2]),
        //         Eigen::Vector3f(255,0,0),
        //         Eigen::Vector3f(0,255,0)
        //     );
        //     rst.DrawLine3D(
        //         Eigen::Vector3f(face->v[2][0], face->v[2][1], face->v[2][2]), 
        //         Eigen::Vector3f(face->v[0][0], face->v[0][1], face->v[0][2]),
        //         Eigen::Vector3f(255,0,0),
        //         Eigen::Vector3f(0,255,0)
        //     );
        // }

        RK_INFO(App, "Begin Render");
        //rst.Draw(tri);
        rst.Draw(triangle_list);
        RK_INFO(App, "End Render");

        // Save Image
        std::string output_path = root + "Data/" + std::to_string(count++) + ".png";
        std::cout << output_path << std::endl;

        // Convert Data
        std::vector<char> img_data;
        img_data.resize(rst.GetWidth() * rst.GetHeight() * 3);
        for(int i = 0; i < rst.GetWidth(); ++i) {
            for(int j = 0; j < rst.GetHeight(); ++j) {
                for(int k = 0; k < 3; ++k) {
                    img_data[(i + j * rst.GetWidth()) * 3 + k] = rst.FrameBuffer()[i + j * rst.GetWidth()][k] * 254;
                }
            }
        }

        stbi_write_png(output_path.c_str(), rst.GetWidth(), rst.GetHeight(), 3, img_data.data(), 0);

        auto data = rst.FrameBuffer().data();
        app.Render(data);
        RK_INFO(App, "Show");
    }

    app.Finalize();

    Log::End();
    return 0;
}
