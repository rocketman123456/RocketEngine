#include "Window/DesktopWindow.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <exception>

namespace Rocket {
    int32_t DesktopWindow::Initialize(const WindowInfo& info) {
        // Copy Info
        info_ = info;
        // info_.width = info.width;
        // info_.height = info.height;
        // info_.name = info.name;
        // info_.resizeable = info.resizeable;
        // info_.fullscreen = info.fullscreen;

        glfwInit();

#if defined(RK_OPENGL) || defined(RK_SOFT)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, false);
#if defined(RK_MACOS)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif
#elif defined(RK_VULKAN)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
        glfwWindowHint(GLFW_RESIZABLE, info_.resizeable);

        handle_ = glfwCreateWindow(info_.width, info_.height, info_.name.c_str(), nullptr, nullptr);
        if (handle_ == nullptr) {
            RK_ERROR(Window, "Failed to create GLFW window");
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent((GLFWwindow*)handle_);

        return 0;
    }

    void DesktopWindow::Finalize() {
        glfwDestroyWindow((GLFWwindow*)handle_);
        glfwTerminate();
    }

    void DesktopWindow::Tick(TimeStep dt) {
        glfwPollEvents();
    }

    bool DesktopWindow::GetShouldClose() {
        return glfwWindowShouldClose((GLFWwindow*)handle_);
    }
}
