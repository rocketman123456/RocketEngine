#pragma once
#include <Eigen/Eigen>

static constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_viewport_matrix(float width, float height);
Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos);
Eigen::Matrix4f get_model_matrix(float rotation_angle);
Eigen::Matrix4f get_model_matrix(float angle_y, float angle_z);
Eigen::Matrix4f get_model_matrix(float angle_x, float angle_y, float angle_z);
Eigen::Matrix4f get_orthographic_matrix(float zLeft, float zRight, float zNear, float zFar, float zTop, float zBottom);
Eigen::Matrix4f get_perspective_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);
Eigen::Matrix4f get_rotation_matrix(Eigen::Vector3f axis, float angle_in_rad);
Eigen::Matrix4f get_scale_matrix(Eigen::Vector3f scale);
