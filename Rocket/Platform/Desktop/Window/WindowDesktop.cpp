#include "Desktop/Window/WindowDesktop.h"

namespace Rocket {
    namespace Window {
        int32_t WindowDesktop::Initialize() {
            if(!glfwInit()) {
                RK_CORE_ERROR("Failed to init GLFW");
                glfwTerminate();
                return 1;
            }
#if defined(RK_OPENGL)
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(RK_MACOS)
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#elif defined(RK_VULKAN) || defined(RK_METAL)
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
            window_ = glfwCreateWindow(width_, height_, "Rocket Engine", NULL, NULL);
            if (window_ == nullptr) {
                RK_CORE_ERROR("Failed to create GLFW window");
                glfwTerminate();
                return 1;
            }

#if defined(RK_OPENGL)
            glfwMakeContextCurrent(window_);
#endif
            return 0;
        }

        void WindowDesktop::Finalize() {
            glfwSetWindowShouldClose(window_, true);
            glfwDestroyWindow(window_);
            glfwTerminate();
        }

        void WindowDesktop::Tick(TimeStep ts) {
            glfwPollEvents();
        }
    }
}
