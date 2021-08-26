#include "Common/Window/DesktopWindow.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Rocket {
    int32_t DesktopWindow::Initialize(const WindowInfo& info) {
        info_.width = info.width;
        info_.height = info.height;
        info_.name = info.name;
        info_.resizeable = info.resizeable;

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, false);
#if defined(RK_MACOS)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        handle_ = glfwCreateWindow(info_.width, info_.height, info_.name.c_str(), nullptr, nullptr);
        if (handle_ == nullptr) {
            RK_ERROR(Window, "Failed to create GLFW window");
            glfwTerminate();
            throw std::exception("Failed to create GLFW window");
        }
        glfwMakeContextCurrent((GLFWwindow*)handle_);

        return 0;
    }

    void DesktopWindow::Finalize() {

    }
}
