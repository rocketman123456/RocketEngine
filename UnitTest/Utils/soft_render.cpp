#include "soft_render.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <iomanip>

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 ourColor;
    out vec2 TexCoord;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        ourColor = aColor;
        // Invert Y coord for opengl corrent rendering
        TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 ourColor;
    in vec2 TexCoord;

    // texture sampler
    uniform sampler2D soft_texture;

    // texture color
    uniform vec4 uniColor;

    void main()
    {
        //FragColor = uniColor;
        FragColor = texture(soft_texture, TexCoord);// * uniColor;
    }
)";

float global_angle_x = 0;
float global_angle_y = 0;
float global_angle_z = 0;

void RenderApp::Initialize(int w, int h) {
    info.name = "Rocket";
    info.width = w;
    info.height = h;
    info.resizeable = false;
    info.fullscreen = false;
    window.Initialize(info);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetKeyCallback((GLFWwindow*)window.GetWindowHandle(), 
        [](GLFWwindow* window, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) { global_angle_z += 5.0; }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) { global_angle_z -= 5.0; }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) { global_angle_y += 5.0; }
        if (key == GLFW_KEY_E && action == GLFW_PRESS) { global_angle_y -= 5.0; }
        if (key == GLFW_KEY_W && action == GLFW_PRESS) { global_angle_x += 5.0; }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) { global_angle_x -= 5.0; }
    });

    // Show OpenGL Info
    std::cout << std::setw(34) << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Shading Language Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Vendor:" << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Renderer:" << (char *)glGetString(GL_RENDERER) << std::endl;

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int32_t success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // load and create a texture 
    // -------------------------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderApp::Tick() {
    window.Tick(10);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderApp::Render(void* data) {
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    float redValue = (cos(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "uniColor");
    int textureLocation = glGetUniformLocation(shaderProgram, "soft_texture");

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGB, GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
    // draw our first triangle
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0); // no need to unbind it every time 

    // TODO : make clear where to swap buffer
    glfwSwapBuffers((GLFWwindow*)window.GetWindowHandle());
}

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

Eigen::Matrix4f get_model_matrix(float angle_x, float angle_y, float angle_z) {
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f rot_x = Eigen::Matrix4f::Zero();
    Eigen::Matrix4f rot_y = Eigen::Matrix4f::Zero();
    Eigen::Matrix4f rot_z = Eigen::Matrix4f::Zero();

    float t1 = angle_x / 180.0 * Rocket::MATH_PI;
    float t2 = angle_y / 180.0 * Rocket::MATH_PI;
    float t3 = angle_z / 180.0 * Rocket::MATH_PI;
    rot_x <<
        1, 0, 0, 0,
        0, cos(t1), sin(t1), 0,
        0, -sin(t1), cos(t1), 0,
        0, 0, 0, 1;
    rot_y << 
        cos(t2), 0, -sin(t2), 0,
        0, 1, 0, 0,
        sin(t2), 0, cos(t2), 0,
        0, 0, 0, 1;
    rot_z << 
        cos(t3),-sin(t3), 0, 0,
        sin(t3), cos(t3), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    model = rot_x * rot_y * rot_z;

    return model;
}

Eigen::Matrix4f get_model_matrix(float angle_y, float angle_z) {
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f rot_y = Eigen::Matrix4f::Zero();
    Eigen::Matrix4f rot_z = Eigen::Matrix4f::Zero();

    float t1 = angle_y / 180.0 * Rocket::MATH_PI;
    float t2 = angle_z / 180.0 * Rocket::MATH_PI;
    rot_y << 
        cos(t1), 0, -sin(t1), 0,
        0, 1, 0, 0,
        sin(t1), 0, cos(t1), 0,
        0, 0, 0, 1;
    rot_z << 
        cos(t2),-sin(t2), 0, 0,
        sin(t2), cos(t2), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    model = rot_y * rot_z;

    return model;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle) {
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    float t = rotation_angle / 180.0 * Rocket::MATH_PI;
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
        2.0 / range_x, 0, 0, -2.0 * center_x / range_x,
        0, 2.0 / range_y, 0, -2.0 * center_y / range_y,
        0, 0, 2.0 / range_z, -2.0 * center_z / range_z,
        0, 0, 0, 1;

    return orthographic;
}

Eigen::Matrix4f get_perspective_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar) {
    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero();

    const float zRange = (zFar - zNear);
    const float tanHalfFOV = tanf((eye_fov / 2.0f / 180.0f * Rocket::MATH_PI));
    const float A = zNear + zFar;
    const float B =-zFar * zNear;

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

Eigen::Vector3f vertex_shader(const Rocket::VertexShaderPayload& payload) {
    return payload.position;
}

Eigen::Vector3f normal_fragment_shader(const Rocket::FragmentShaderPayload& payload) {
    Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
    return return_color * 255.0f;
}

Eigen::Vector3f reflect(const Eigen::Vector3f& vec, const Eigen::Vector3f& axis) {
    auto costheta = vec.dot(axis);
    return (2 * costheta * axis - vec).normalized();
}

Eigen::Vector3f texture_fragment_shader(const Rocket::FragmentShaderPayload& payload) {
    Eigen::Vector3f return_color = {0, 0, 0};
    if (payload.texture.get() != nullptr) {
        // Get the texture value at the texture coordinates of the current fragment
        return_color = payload.texture->GetColorRGB(payload.tex_coords.x(), payload.tex_coords.y());
    }
    Eigen::Vector3f texture_color;
    texture_color << return_color.x(), return_color.y(), return_color.z();

    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = texture_color / 255.f;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = texture_color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;
    normal.normalize();

    Eigen::Vector3f result_color = {0, 0, 0};

    for (auto& light : lights) {
        Eigen::Vector3f light_dir = light.position - point;
        float dist_square = light_dir.dot(light_dir);
        Eigen::Vector3f view_dir = eye_pos - point;
        light_dir.normalize();
        view_dir.normalize();
        Eigen::Vector3f h = (light_dir + view_dir).normalized();
        Eigen::Vector3f Ld = Eigen::Vector3f::Zero();
        Eigen::Vector3f Ls = Eigen::Vector3f::Zero();
        Eigen::Vector3f La = Eigen::Vector3f::Zero();
        Ld = kd.cwiseProduct(light.intensity / dist_square) * std::max(0.0f, normal.dot(light_dir));
        Ls = ks.cwiseProduct(light.intensity / dist_square) * std::pow(std::max(0.0f, normal.dot(h)), p);
        La = ka.cwiseProduct(amb_light_intensity);
        result_color += (Ld + Ls + La);
    }

    return result_color * 255.f;
}

Eigen::Vector3f phong_fragment_shader(const Rocket::FragmentShaderPayload& payload) {
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color;
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;
    normal.normalize();

    Eigen::Vector3f result_color = {0, 0, 0};
    for (auto& light : lights) {
        Eigen::Vector3f light_dir = light.position - point;
        float dist_square = light_dir.dot(light_dir);
        Eigen::Vector3f view_dir = eye_pos - point;
        light_dir.normalize();
        view_dir.normalize();
        Eigen::Vector3f h = (light_dir + view_dir).normalized();
        Eigen::Vector3f Ld = Eigen::Vector3f::Zero();
        Eigen::Vector3f Ls = Eigen::Vector3f::Zero();
        Eigen::Vector3f La = Eigen::Vector3f::Zero();
        Ld = kd.cwiseProduct(light.intensity / dist_square) * std::max(0.0f, normal.dot(light_dir));
        Ls = ks.cwiseProduct(light.intensity / dist_square) * std::pow(std::max(0.0f, normal.dot(h)), p);
        La = ka.cwiseProduct(amb_light_intensity);
        result_color += (Ld + Ls + La);
    }

    return result_color * 255.f;
}

Eigen::Vector3f displacement_fragment_shader(const Rocket::FragmentShaderPayload& payload) {
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    float kh = 0.2, kn = 0.1;
    
    Eigen::Vector3f n = normal;
    Eigen::Vector3f t(n[0]*n[1] / sqrt(n[0]*n[0]+n[2]*n[2]), sqrt(n[0]*n[0]+n[2]*n[2]), n[2]*n[1] / sqrt(n[0]*n[0]+n[2]*n[2]));
    Eigen::Vector3f b = n.cross(t);
    Eigen::Matrix3f TBN;
    TBN.block<3,1>(0,0) = t;
    TBN.block<3,1>(0,1) = b;
    TBN.block<3,1>(0,2) = n;
    float u = payload.tex_coords.x();
	float v = payload.tex_coords.y();
	float w = payload.texture->width;
	float h = payload.texture->height;
    float dU = kh * kn * (payload.texture->GetColorRGB(u + 1.0f / w , v).norm() - payload.texture->GetColorRGB(u, v).norm());
	float dV = kh * kn * (payload.texture->GetColorRGB(u, v + 1.0f / h).norm() - payload.texture->GetColorRGB(u, v).norm());
    Eigen::Vector3f ln(-dU, -dV, 1);
    n = TBN * ln;
    
    point += (kn * normal * payload.texture->GetColorRGB(u , v).norm());
    normal = n.normalized();

    Eigen::Vector3f result_color = {0, 0, 0};

    for (auto& light : lights) {
        Eigen::Vector3f light_dir = light.position - point;
        float dist_square = light_dir.dot(light_dir);
        Eigen::Vector3f view_dir = eye_pos - point;
        light_dir.normalize();
        view_dir.normalize();
        Eigen::Vector3f h = (light_dir + view_dir).normalized();
        Eigen::Vector3f Ld = Eigen::Vector3f::Zero();
        Eigen::Vector3f Ls = Eigen::Vector3f::Zero();
        Eigen::Vector3f La = Eigen::Vector3f::Zero();
        Ld = kd.cwiseProduct(light.intensity / dist_square) * std::max(0.0f, normal.dot(light_dir));
        Ls = ks.cwiseProduct(light.intensity / dist_square) * std::pow(std::max(0.0f, normal.dot(h)), p);
        La = ka.cwiseProduct(amb_light_intensity);
        result_color += (Ld + Ls + La);
    }

    return result_color * 255.f;
}

Eigen::Vector3f bump_fragment_shader(const Rocket::FragmentShaderPayload& payload) {
    
    Eigen::Vector3f ka = Eigen::Vector3f(0.005, 0.005, 0.005);
    Eigen::Vector3f kd = payload.color;
    Eigen::Vector3f ks = Eigen::Vector3f(0.7937, 0.7937, 0.7937);

    auto l1 = light{{20, 20, 20}, {500, 500, 500}};
    auto l2 = light{{-20, 20, 0}, {500, 500, 500}};

    std::vector<light> lights = {l1, l2};
    Eigen::Vector3f amb_light_intensity{10, 10, 10};
    Eigen::Vector3f eye_pos{0, 0, 10};

    float p = 150;

    Eigen::Vector3f color = payload.color; 
    Eigen::Vector3f point = payload.view_pos;
    Eigen::Vector3f normal = payload.normal;

    float kh = 0.2, kn = 0.1;

    Eigen::Vector3f n = normal;
    Eigen::Vector3f t(n[0]*n[1] / sqrt(n[0]*n[0]+n[2]*n[2]), sqrt(n[0]*n[0]+n[2]*n[2]), n[2]*n[1] / sqrt(n[0]*n[0]+n[2]*n[2]));
    Eigen::Vector3f b = n.cross(t);
    Eigen::Matrix3f TBN;
    TBN.block<3,1>(0,0) = t;
    TBN.block<3,1>(0,1) = b;
    TBN.block<3,1>(0,2) = n;
    float u = payload.tex_coords.x();
	float v = payload.tex_coords.y();
	float w = payload.texture->width;
	float h = payload.texture->height;
    float dU = kh * kn * (payload.texture->GetColorRGB(u + 1.0f / w , v).norm() - payload.texture->GetColorRGB(u, v).norm());
	float dV = kh * kn * (payload.texture->GetColorRGB(u, v + 1.0f / h).norm() - payload.texture->GetColorRGB(u, v).norm());
    Eigen::Vector3f ln(-dU, -dV, 1);
    n = TBN * ln;
    normal = n.normalized();

    Eigen::Vector3f result_color = {0, 0, 0};
    result_color = normal;

    return result_color * 255.f;
}
