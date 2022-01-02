#include "Desktop/DesktopWindow.h"
#include "Log/Log.h"

#include <exception>
#include <stdexcept>

namespace Rocket {
    void DesktopWindow::Initialize() {
        if (!glfwInit()) {
            RK_ERROR(Window, "Failed to Initialize GLFW");
            throw std::runtime_error("Failed to Initialize GLFW");
        }
#if defined(RK_VULKAN)
        if (glfwVulkanSupported()) {
            RK_INFO(Window, "Vulkan Supported");
        } else {
            RK_ERROR(Window, "Vulkan Not Supported");
            throw std::runtime_error("Vulkan Not Supported");
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#elif defined(RK_OPENGL)
        RK_INFO(Window, "OpenGL 4.6 Supported");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#elif defined(RK_SOFT)
        RK_INFO(Window, "OpenGL 4.1 Supported");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#endif
        window = glfwCreateWindow(Width(), Height(), Title().c_str(), nullptr, nullptr);
        if (!window) {
            RK_ERROR(Window, "Failed to initialize glfw window");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwSetKeyCallback(
            window,
            [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        );
    }

    void DesktopWindow::Finalize() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void DesktopWindow::Tick() {
        glfwPollEvents();
    }

    bool DesktopWindow::IsRunning() {
        return !glfwWindowShouldClose(window);
    }
}
