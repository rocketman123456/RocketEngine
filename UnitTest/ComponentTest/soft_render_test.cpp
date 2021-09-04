#include "Window/DesktopWindow.h"
#include "Log/Log.h"

using namespace Rocket;

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

int main(int argc, char** argv) {
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
    
    while(!window.GetShouldClose()) {
        window.Tick(10);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO : make clear where to swap buffer
        glfwSwapBuffers((GLFWwindow*)window.GetWindowHandle());
    }

    window.Finalize();
    return 0;
}