#pragma once
#include "Math/Parameter.h"
#include "Render/SoftShader.h"
#include "Window/DesktopWindow.h"
#include <Eigen/Eigen>

extern float global_angle_x;
extern float global_angle_y;
extern float global_angle_z;

class RenderApp {
public:
    void Initialize(int w, int h);
    bool ShouldClose() { return window.GetShouldClose(); }
    void Tick();
    void Render(void* data);
    void Finalize() { window.Finalize(); }

private:
    Rocket::DesktopWindow window;
    Rocket::WindowInfo info;
    //int vertexColorLocation;
    //int textureLocation;
    unsigned int texture;
    unsigned int VBO, VAO, EBO;
    unsigned int shaderProgram;
    unsigned int fragmentShader;
    unsigned int vertexShader;
};

Eigen::Matrix4f get_viewport_matrix(float width, float height);
Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos);
Eigen::Matrix4f get_model_matrix(float rotation_angle);
Eigen::Matrix4f get_model_matrix(float angle_y, float angle_z);
Eigen::Matrix4f get_model_matrix(float angle_x, float angle_y, float angle_z);
Eigen::Matrix4f get_orthographic_matrix(float zLeft, float zRight, float zNear, float zFar, float zTop, float zBottom);
Eigen::Matrix4f get_perspective_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);
Eigen::Matrix4f get_rotation_matrix(Eigen::Vector3f axis, float angle_in_rad);
Eigen::Matrix4f get_scale_matrix(Eigen::Vector3f scale);

struct light {
    Eigen::Vector3f position;
    Eigen::Vector3f intensity;
};

Eigen::Vector3f vertex_shader(const Rocket::VertexShaderPayload& payload);
Eigen::Vector3f normal_fragment_shader(const Rocket::FragmentShaderPayload& payload);
Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis);
Eigen::Vector3f texture_fragment_shader(const Rocket::FragmentShaderPayload& payload);
Eigen::Vector3f phong_fragment_shader(const Rocket::FragmentShaderPayload& payload);
Eigen::Vector3f displacement_fragment_shader(const Rocket::FragmentShaderPayload& payload);
Eigen::Vector3f bump_fragment_shader(const Rocket::FragmentShaderPayload& payload);
