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
#elif defined(RK_OPENGL)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif defined(RK_SOFT)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    }

    void DesktopWindow::Finalize() {
        
    }

    void DesktopWindow::Tick() {
        glfwPollEvents();
    }
}
