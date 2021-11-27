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
    //Log::Init();

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

    std::vector<Eigen::Vector3f> pos
        {
            {2, 0, -2},
            {0, 2, -2},
            {-2, 0, -2},
            {3.5, -1, -5},
            {2.5, 1.5, -5},
            {-1, 0.5, -5}
        };

    std::vector<Eigen::Vector3i> ind
        {
            {0, 1, 2},
            {3, 4, 5}
        };

    std::vector<Eigen::Vector3f> cols
        {
            {217.0, 238.0, 185.0},
            {217.0, 238.0, 185.0},
            {217.0, 238.0, 185.0},
            {185.0, 217.0, 238.0},
            {185.0, 217.0, 238.0},
            {185.0, 217.0, 238.0}
        };

    auto pos_id = rst.LoadPositions(pos);
    auto ind_id = rst.LoadIndices(ind);
    auto col_id = rst.LoadColors(cols);

    rst.DisableWireFrame();
    //rst.EnableWireFrame();
    //rst.EnableMsaa();
    //rst.SetMsaaLevel(0);

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
        std::cout << "Set MVP" << std::endl;

        rst.DrawLine3D({0,0,0}, {1,0,0}, {255,0,0}, {255,0,0}); // x
        rst.DrawLine3D({0,0,0}, {0,1,0}, {0,255,0}, {0,255,0}); // y
        rst.DrawLine3D({0,0,0}, {0,0,1}, {0,0,255}, {0,0,255}); // z

        rst.Draw(pos_id, ind_id, col_id, RenderPrimitive::TRIANGLE);
        //rst.Draw(pos_id, ind_id, RenderPrimitive::TRIANGLE);

        auto data = rst.FrameBuffer().data();
        app.Render(data);
        RK_INFO(App, "Show");
    }

    app.Finalize();

    //Log::End();
    return 0;
}
