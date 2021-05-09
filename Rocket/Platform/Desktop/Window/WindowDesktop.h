#pragma once
//#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#if defined(RK_OPENGL)
#include <glad/glad.h>
#elif defined(RK_VULKAN)

#elif defined(RK_METAL)

#endif

#include "Core/Core.h"
#include "Utils/TimeStep.h"

namespace Rocket {
    namespace Window {
        class WindowDesktop {
        public:
            [[nodiscard]] int32_t Initialize();
            void Finalize();
            void Tick(TimeStep ts);

            [[nodiscard]] inline void* GetWindowHandle() { return window_; }
        private:
            GLFWwindow* window_ = nullptr;
            int32_t width_ = 1280;
            int32_t height_ = 720;
        };
    }
}