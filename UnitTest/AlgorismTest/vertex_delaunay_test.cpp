#include "Log/Log.h"
#include "Window/DesktopWindow.h"
#include "Render/SoftRasterizer.h"
#include "Render/SoftTriangle.h"

#include "Geometry/Vertex.h"
#include "Geometry/Triangle.h"
#include "Geometry/Sphere.h"
#include "Geometry/Tetrahedra.h"
#include "Geometry/MeshOperation/Delaunay3D.h"

using namespace Rocket;

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <iomanip>
#include <random>

#include "../Utils/soft_render.h"

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

static float global_angle_x = 0;
static float global_angle_y = 0;
static float global_angle_z = 0;
static std::mt19937 generator(0);
static std::uniform_real_distribution<> range_distribute(0.0, 1.0);
static std::vector<VertexPtr> nodes;
static SpherePtr sphere;
static TetrahedraPtr tetrahedra;
static Delaunay3DPtr delaunay_3d;

double random(double a = 0.0, double b = 1.0) {
    return range_distribute(generator) * (b-a) + a;
}

void VertexSphereTest() {
    int32_t count = 0;
    while(count <= 0) {
        std::cout << "Input node count:";
        std::cin >> count;
    }

    double radius = 1.0;
    for(int i = 0; i < count; i++) {
        double t1 = random(-3.14/2.0,3.14/2.0);
        double t2 = random(0,3.14*2.0);
        double x = radius * sin(t1) * sin(t2);
        double y = radius * sin(t1) * cos(t2);
        double z = radius * cos(t1);
        //nodes.push_back(VertexPtr(new Vertex(Eigen::Vector3d(x, y, z))));
        nodes.push_back(VertexPtr(new Vertex(Eigen::Vector3d(random(-1,1), random(-1,1), random(-1,1)))));
    }

    sphere = SpherePtr(new Sphere());
    sphere->CreateBoundingSphere(nodes);

    std::cout << "Sphere Center: " 
        << sphere->center[0] << "," 
        << sphere->center[1] << "," 
        << sphere->center[2] << std::endl;
    std::cout << "Sphere Radius: " << sphere->radius << std::endl;

    tetrahedra = TetrahedraPtr(new Tetrahedra());
    tetrahedra->CreateBoundingTetrahedra(
        sphere,
        Eigen::Vector3d(0,0,1),
        Eigen::Vector3d(1,0,0),
        Eigen::Vector3d(0,1,0)
    );

    delaunay_3d = Delaunay3DPtr(new Delaunay3D());
    delaunay_3d->method = 1;
    delaunay_3d->Initialize(nodes);
}

int main(int argc, char** argv) {
    Log::Init();

    DesktopWindow window;
    WindowInfo info;
    info.name = "Rocket";
    info.width = 1280;
    info.height = 720;
    info.resizeable = false;
    info.fullscreen = false;
    window.Initialize(info);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Show OpenGL Info
    std::cout << std::setw(34) << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Shading Language Version: " << (char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Vendor:" << (char *)glGetString(GL_VENDOR) << std::endl;
    std::cout << std::setw(34) << std::left << "OpenGL Renderer:" << (char *)glGetString(GL_RENDERER) << std::endl;

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
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

    unsigned int VBO, VAO, EBO;
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
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    // load image, create texture and generate mipmaps
    int32_t width = info.width;
    int32_t height = info.height;
    int32_t nrChannels = 4;

    SoftRasterizer rst(info.width, info.height);
    rst.ClearAll(BufferType::COLOR | BufferType::DEPTH);

    Eigen::Vector3f eye_pos = {0.0, 0.0, 5};

    int32_t key = 0;
    int32_t frame_count = 0;

    glfwSetKeyCallback((GLFWwindow*)window.GetWindowHandle(), 
        [](GLFWwindow* window, int key, int scancode, int action, int mods){
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { delaunay_3d->Generate(); }
        if (key == GLFW_KEY_A && action == GLFW_PRESS) { global_angle_z += 5.0; }
        if (key == GLFW_KEY_D && action == GLFW_PRESS) { global_angle_z -= 5.0; }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) { global_angle_y += 5.0; }
        if (key == GLFW_KEY_E && action == GLFW_PRESS) { global_angle_y -= 5.0; }
        if (key == GLFW_KEY_W && action == GLFW_PRESS) { global_angle_x += 5.0; }
        if (key == GLFW_KEY_S && action == GLFW_PRESS) { global_angle_x -= 5.0; }
    });

    rst.DisableWireFram();
    rst.EnableWireFram();
    rst.EnableMsaa();
    rst.SetMsaaLevel(0);

    VertexSphereTest();
    
    while(!window.GetShouldClose()) {
        window.Tick(10);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        float redValue = (cos(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "uniColor");
        int textureLocation = glGetUniformLocation(shaderProgram, "soft_texture");

        rst.NextFrame();
        rst.Clear(BufferType::COLOR | BufferType::DEPTH);
        rst.SetModel(get_model_matrix(global_angle_x, global_angle_y, global_angle_z));
        //rst.SetModel(get_model_matrix(global_angle_y, global_angle_z));
        //rst.SetModel(get_model_matrix(global_angle_z));
        rst.SetView(get_view_matrix(eye_pos));
        rst.SetProjection(get_perspective_matrix(45, ((float)info.width/(float)info.height), 0.1, 50));
        //rst.SetProjection(get_orthographic_matrix(-6.4, 6.4, -50, 50, 3.6, -3.6));

        rst.DrawLine3D({0,0,0}, {1,0,0}, {255,0,0}, {255,0,0}); // x
        rst.DrawLine3D({0,0,0}, {0,1,0}, {0,255,0}, {0,255,0}); // y
        rst.DrawLine3D({0,0,0}, {0,0,1}, {0,0,255}, {0,0,255}); // z
        //rst.DrawLine3D({0.5,0.5,0}, {0.5,0.5,2}, {255,0,0}, {0,255,0});
        rst.DrawPoint3D({1,1,0}, {255,0,0});

        for(auto& node : delaunay_3d->GetNodes()) {
            rst.DrawPoint3D(Eigen::Vector3f(node->position[0], node->position[1], node->position[2]));
        }

        std::vector<TetrahedraPtr>& meshs = delaunay_3d->GetResultTetrahedras();

        // for(TetrahedraPtr& mesh : meshs) {
        //     mesh->UpdateFaces();
        //     std::array<TrianglePtr, 4>& faces = mesh->faces;
        //     for(TrianglePtr& face : faces) {
        //         std::array<EdgePtr, 3>& edges = face->edges;
        //         for(EdgePtr& edge : edges) {
        //             rst.DrawLine3D(
        //                 Eigen::Vector3f(edge->start->position[0], edge->start->position[1], edge->start->position[2]), 
        //                 Eigen::Vector3f(edge->end->position[0], edge->end->position[1], edge->end->position[2]),
        //                 Eigen::Vector3f(255,0,0),
        //                 Eigen::Vector3f(0,0,255)
        //             );
        //         }
        //     }
        // }

        static int32_t current = 0;
        if(meshs.size() > 0) {
            current++;
            current = current % meshs.size();
            auto mesh = meshs[current];
            mesh->UpdateFaces();
            std::array<TrianglePtr, 4>& faces = mesh->faces;
            for(TrianglePtr& face : faces) {
                std::array<EdgePtr, 3>& edges = face->edges;
                for(EdgePtr& edge : edges) {
                    rst.DrawLine3D(
                        Eigen::Vector3f(edge->start->position[0], edge->start->position[1], edge->start->position[2]), 
                        Eigen::Vector3f(edge->end->position[0], edge->end->position[1], edge->end->position[2]),
                        Eigen::Vector3f(255,0,0),
                        Eigen::Vector3f(0,0,255)
                    );
                }
            }
        }
        

        // auto& faces = tetrahedra.faces;
        // for(auto face : faces) {
        //     auto& edges = face.edges;
        //     for(auto edge : edges) {
        //         rst.DrawLine3D(
        //             Eigen::Vector3f(edge.first.position[0], edge.first.position[1], edge.first.position[2]), 
        //             Eigen::Vector3f(edge.second.position[0], edge.second.position[1], edge.second.position[2]),
        //             Eigen::Vector3f(255,0,0),
        //             Eigen::Vector3f(0,0,255)
        //         );
        //     }
        // }

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, rst.FrameBuffer().data());
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

    window.Finalize();

    Log::End();
    return 0;
}
