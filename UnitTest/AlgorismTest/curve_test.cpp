#include "Log/Log.h"
#include "Window/DesktopWindow.h"
#include "Render/SoftRasterizer.h"
#include "Render/SoftTriangle.h"
#include "Geometry/BezierCurve.h"
#include "Geometry/BezierSurface.h"
#include "Parser/ObjParser.h"
#include "Utils/FindRootDir.h"

#include "Geometry/Sphere.h"

using namespace Rocket;

#include "../Utils/soft_render.h"

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

    Eigen::Vector3f cp[4][4];

    int32_t count = 20;
    double dt = 1.0 / (double)count;
    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Vector3i> indices;

    pos_buf_id pos;
    ind_buf_id ind;

    //rst.DisableWireFrame();
    rst.EnableWireFrame();
    rst.DisableMsaa();
    //rst.EnableMsaa();
    //rst.SetMsaaLevel(1);
    //rst.EnableShader();

    int32_t k = 0;
    RK_INFO(App, "Initialize Finished");

    double time = 0.0;
    while(!app.ShouldClose()) {
        app.Tick();

        rst.NextFrame();
        rst.Clear(BufferType::COLOR | BufferType::DEPTH);
        rst.SetModel(get_model_matrix(global_angle_x, global_angle_y, global_angle_z));
        rst.SetView(get_view_matrix(eye_pos));
        rst.SetProjection(get_perspective_matrix(45, ((float)width/(float)height), 0.1, 50));
        //rst.SetProjection(get_orthographic_matrix(-6.4, 6.4, -50, 50, 3.6, -3.6));

        rst.DrawLine3D({0,0,0}, {1,0,0}, {255,0,0}, {255,0,0}); // x
        rst.DrawLine3D({0,0,0}, {0,1,0}, {0,255,0}, {0,255,0}); // y
        rst.DrawLine3D({0,0,0}, {0,0,1}, {0,0,255}, {0,0,255}); // z

        cp[0][0] << 0.2 * sin(time),      0,              0;
        cp[0][1] << 0.2 * sin(time) + 0.25,   sin(time),     0;
        cp[0][2] << 0.2 * sin(time) + 0.75,   cos(time),     0;
        cp[0][3] << 0.2 * sin(time) + 1.0,      0,              0;

        cp[1][0] << 0.2 * sin(time + MATH_PI_4),      0,              0.25;
        cp[1][1] << 0.2 * sin(time + MATH_PI_4) + 0.25,   sin(time + MATH_PI_4),     0.25;
        cp[1][2] << 0.2 * sin(time + MATH_PI_4) + 0.75,   cos(time + MATH_PI_4),     0.25;
        cp[1][3] << 0.2 * sin(time + MATH_PI_4) + 1.0,      0,              0.25;

        cp[2][0] << 0.2 * sin(time + MATH_PI_4 * 1.5),      0,              0.5;
        cp[2][1] << 0.2 * sin(time + MATH_PI_4 * 1.5) + 0.25,   sin(time + MATH_PI_4 * 2.0),     0.5;
        cp[2][2] << 0.2 * sin(time + MATH_PI_4 * 1.5) + 0.75,   cos(time + MATH_PI_4 * 2.0),     0.5;
        cp[2][3] << 0.2 * sin(time + MATH_PI_4 * 1.5) + 1.0,      0,              1;

        cp[3][0] << 0.2 * sin(time + MATH_PI_4 * 2.0),      0,              0.75;
        cp[3][1] << 0.2 * sin(time + MATH_PI_4 * 2.0) + 0.25,   sin(time + MATH_PI_4 * 3.0),     0.75;
        cp[3][2] << 0.2 * sin(time + MATH_PI_4 * 2.0) + 0.75,   cos(time + MATH_PI_4 * 3.0),     0.75;
        cp[3][3] << 0.2 * sin(time + MATH_PI_4 * 2.0) + 1.0,      0,              0.75;

        time += 0.1;

        std::vector<std::vector<Eigen::Vector3f>> cps(
            {{cp[0][0], cp[0][1], cp[0][2], cp[0][3]},
            {cp[1][0], cp[1][1], cp[1][2], cp[1][3]},
            {cp[2][0], cp[2][1], cp[2][2], cp[2][3]},
            {cp[2][0], cp[2][1], cp[2][2], cp[2][3]}}
        );

        vertices.clear();
        for(int32_t i = 0; i < count; ++i) {
            for(int32_t j = 0; j < count; ++j) {
                double x = (double)i * dt;
                double y = (double)j * dt;
                vertices.push_back(Geometry::CalculateBezierSurface(cps, x, y));
            }
        }
        Geometry::GenerateBezierSurface(vertices, count, count, indices);

        pos = rst.LoadPositions(vertices);
        ind = rst.LoadIndices(indices);

        rst.Draw(pos, ind, RenderPrimitive::TRIANGLE);
        
        rst.UnloadPositions(pos);
        rst.UnloadIndices(ind);

        auto data = rst.FrameBuffer().data();
        app.Render(data);
    }

    app.Finalize();

    Log::End();
    return 0;
}
