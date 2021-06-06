#include "OpenGL/OpenGLLoader.h"

#include <glad/glad.h>

namespace Rocket {
    int32_t OpenGLLoader::LoadDriver() {
        //if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        //    RK_GRAPHICS_ERROR("Failed to initialize GLAD");
        //    return 1;
        //}
        return 0;
    }

    //void OpenGLLoader::UnloadDriver() {
    //}
    //int32_t OpenGLLoader::LoadDevice() {
    //    return 0;
    //}
    //void OpenGLLoader::UnloadDevice() {
    //}
    //int32_t OpenGLLoader::LoadEngine() {
    //    return 0;
    //}
    //void OpenGLLoader::UnloadEngine() {
    //}
}
