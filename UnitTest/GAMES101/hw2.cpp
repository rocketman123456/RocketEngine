#include "Log/Log.h"
#include "Window/DesktopWindow.h"
#include "Render/SoftRasterizer.h"
#include "Render/SoftTriangle.h"
#include "Parser/ObjParser.h"
#include "Utils/FindRootDir.h"

#include "Geometry/Sphere.h"

using namespace Rocket;

#include "../Utils/soft_render.h"

#include <iostream>

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

    std::string model_path = "Asset/Model/rock/rock.obj";
    std::string texture_path = "Asset/Model/rock/rock.png";

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

    std::function<Eigen::Vector3f(FragmentShaderPayload)> fragment_shader = phong_fragment_shader;

    rst.SetTexture(texture);
    rst.SetVertexShader(vertex_shader);
    rst.SetFragmentShader(fragment_shader);
    rst.DisableWireFrame();
    //rst.EnableWireFrame();
    //rst.EnableMsaa();
    //rst.SetMsaaLevel(0);

    std::cout << "Initialize Finished" << std::endl;
    
    while(!app.ShouldClose()) {
        app.Tick();

        rst.NextFrame();
        rst.Clear(BufferType::COLOR | BufferType::DEPTH);
        rst.SetModel(get_model_matrix(global_angle_x, global_angle_y, global_angle_z));
        rst.SetView(get_view_matrix(eye_pos));
        rst.SetProjection(get_perspective_matrix(45, ((float)width/(float)height), 0.1, 50));
        //rst.SetProjection(get_orthographic_matrix(-6.4, 6.4, -50, 50, 3.6, -3.6));
        std::cout << "Set MVP" << std::endl;

        rst.DrawLine3D({0,0,0}, {1,0,0}, {255,0,0}, {255,0,0}); // x
        rst.DrawLine3D({0,0,0}, {0,1,0}, {0,255,0}, {0,255,0}); // y
        rst.DrawLine3D({0,0,0}, {0,0,1}, {0,0,255}, {0,0,255}); // z

        RK_INFO(App, "Begin Render");
        rst.Draw(triangle_list);
        RK_INFO(App, "End Render");

        auto data = rst.FrameBuffer().data();
        app.Render(data);
        RK_INFO(App, "Show");
    }

    app.Finalize();

    Log::End();
    return 0;
}
