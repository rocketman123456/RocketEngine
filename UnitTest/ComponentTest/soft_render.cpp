#include "soft_render.h"

Eigen::Matrix4f get_viewport_matrix(float width, float height) {
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    // from [-1, 1]^2 -> [0,w] X [0,h]

    view(0, 0) = width / 2.0;
    view(1, 1) = height / 2.0;
    view(0, 3) = width / 2.0;
    view(1, 3) = height / 2.0;

    return view;
}

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos) {
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 
        1, 0, 0, -eye_pos[0], 
        0, 1, 0, -eye_pos[1], 
        0, 0, 1, -eye_pos[2], 
        0, 0, 0, 1;

    // give look_at and up
    // view ^ -1 = [look_at x up; up; -look_at]

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle) {
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float t = rotation_angle / 180.0 * MY_PI;
    model << 
        cos(t),-sin(t), 0, 0,
        sin(t), cos(t), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    return model;
}

Eigen::Matrix4f get_orthographic_matrix(float zLeft, float zRight, float zNear, float zFar, float zTop, float zBottom) {
    Eigen::Matrix4f orthographic = Eigen::Matrix4f::Zero();

    const float center_x = (zRight + zLeft) / 2.0;
    const float center_y = (zTop + zBottom) / 2.0;
    const float center_z = (zNear + zFar) / 2.0;
    const float range_x = (zRight - zLeft);
    const float range_y = (zTop - zBottom);
    const float range_z = (zFar - zNear);

    // Eigen::Matrix4f scale;
    // Eigen::Matrix4f trans;
    // scale << 
    //     2.0 / range_x, 0, 0, 0,
    //     0, 2.0 / range_y, 0, 0,
    //     0, 0, 2.0 / range_z, 0,
    //     0, 0, 0, 1;
    // trans <<
    //     1, 0, 0, -center_x,
    //     0, 1, 0, -center_y,
    //     0, 0, 1, -center_z,
    //     0, 0, 0, 1;
    // orthographic = scale * trans;

    orthographic << 
        1, 0, 0, -2.0 * center_x / range_x,
        0, 1, 0, -2.0 * center_y / range_y,
        0, 0, 1, -2.0 * center_z / range_z,
        0, 0, 0, 1;

    return orthographic;
}

Eigen::Matrix4f get_perspective_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar) {
    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero();

    const float zRange = (zFar - zNear);
    const float tanHalfFOV = tanf((eye_fov / 2.0f / 180.0f * MY_PI));
    const float A = zNear + zFar;
    const float B = - zFar * zNear;

    // Eigen::Matrix4f perspect_to_ortho;
    // Eigen::Matrix4f ortho;

    // perspect_to_ortho << 
    //  zNear, 0, 0, 0,
    //  0, zNear, 0, 0,
    //  0, 0, A, B,
    //  0, 0, 1, 0;

    // float right = aspect_ratio * tanHalfFOV * zNear;
    // float top = tanHalfFOV * zNear;
    // ortho = get_orthographic_matrix(-right, right, zNear, zFar, top, -top);

    // projection = ortho * per;
    
    projection(0,0) = 1.0f / (tanHalfFOV * aspect_ratio); 
    projection(1,1) = 1.0f / tanHalfFOV;   
    projection(2,2) = -A / zRange;  
    projection(2,3) = -2.0f * B / zRange;
    projection(3,2) = -1.0f;            

    return projection;
}

Eigen::Matrix4f get_rotation_matrix(Eigen::Vector3f axis, float angle_in_rad) {
    Eigen::Matrix4f rotation = Eigen::Matrix4f::Zero();

    Eigen::Matrix3f axis_mat = axis * axis.transpose();

    Eigen::Matrix3f axis_inv_mat = Eigen::Matrix3f::Zero();
    axis_inv_mat(0,1) = -axis[2];
    axis_inv_mat(1,0) = axis[2];
    axis_inv_mat(0,2) = axis[1];
    axis_inv_mat(2,0) = -axis[1];
    axis_inv_mat(1,2) = -axis[0];
    axis_inv_mat(2,1) = axis[0];

    float c1 = cos(angle_in_rad);
    float c1_ = 1 - cos(angle_in_rad);
    float s1 = sin(angle_in_rad);

    Eigen::Matrix3f rot = c1 * Eigen::Matrix3f::Identity() + c1_ * axis_mat + s1 * axis_inv_mat;

    rotation(3,3) = 1;
    rotation.block<3,3>(0,0) = rot;

    return rotation;
}

Eigen::Matrix4f get_scale_matrix(Eigen::Vector3f scale) {
    Eigen::Matrix4f scale_mat = Eigen::Matrix4f::Zero();

    scale_mat(3,3) = 1;
    scale_mat(0,0) = scale[0];
    scale_mat(1,1) = scale[1];
    scale_mat(2,2) = scale[2];

    return scale_mat;
}
