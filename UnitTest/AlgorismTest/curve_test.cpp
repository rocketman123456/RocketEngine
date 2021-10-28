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

    cp[0][0] << 0, 0, 0;
    cp[0][1] << 0.25, 1, 0;
    cp[0][2] << 0.75, -1, 0;
    cp[0][3] << 1, 0, 0;

    cp[1][0] << 0, 0, 0.25;
    cp[1][1] << 0.25, -1, 0.25;
    cp[1][2] << 0.75, 1, 0.25;
    cp[1][3] << 1, 0, 0.25;

    cp[2][0] << 0, 0, 0.5;
    cp[2][1] << 0.25, 1, 0.5;
    cp[2][2] << 0.75, -1, 0.5;
    cp[2][3] << 1, 0, 1;

    cp[3][0] << 0, 0, 0.75;
    cp[3][1] << 0.25, -1, 0.75;
    cp[3][2] << 0.75, 1, 0.75;
    cp[3][3] << 1, 0, 0.75;

    Geometry::BezierCurve curve1({cp[0][0], cp[0][1], cp[0][2], cp[0][3]});
    Geometry::BezierCurve curve2({cp[1][0], cp[1][1], cp[1][2], cp[1][3]});
    Geometry::BezierCurve curve3({cp[2][0], cp[2][1], cp[2][2], cp[2][3]});
    Geometry::BezierCurve curve4({cp[3][0], cp[3][1], cp[3][2], cp[3][3]});

    Geometry::BezierSurface surface({curve1, curve2, curve3, curve4});
    surface.Compute(40);
    surface.GenerateMesh();
    auto& result = surface.GetResult();

    auto& vertices = surface.GetVertices();
    auto& indices = surface.GetIndices();

    auto pos = rst.LoadPositions(vertices);
    auto ind = rst.LoadIndices(indices);

    //rst.DisableWireFrame();
    rst.EnableWireFrame();
    rst.DisableMsaa();
    //rst.EnableMsaa();
    //rst.SetMsaaLevel(1);
    //rst.EnableShader();

    int32_t k = 0;
    RK_INFO(App, "Initialize Finished");
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

        auto& points = result[k++];
        k = k % result.size();
        for(int i = 0; i < points.size() - 1; ++i) {
            rst.DrawLine3D(points[i], points[i+1], {255, 0, 0}, {255, 0, 0});
        }

        // for(int i = 0; i < indices.size(); ++i) {
        //     rst.DrawLine3D(vertices[indices[i][0]], vertices[indices[i][1]], {255, 0, 255}, {255, 255, 0});
        //     rst.DrawLine3D(vertices[indices[i][1]], vertices[indices[i][2]], {255, 0, 255}, {255, 255, 0});
        //     rst.DrawLine3D(vertices[indices[i][2]], vertices[indices[i][0]], {255, 0, 255}, {255, 255, 0});
        // }

        rst.Draw(pos, ind, RenderPrimitive::TRIANGLE);
        //rst.DrawPoints3D(points);

        auto data = rst.FrameBuffer().data();
        app.Render(data);
    }

    app.Finalize();

    Log::End();
    return 0;
}
