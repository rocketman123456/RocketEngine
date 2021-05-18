#include "Core/Core.h"
#include "Module/Application.h"
#include "Module/WindowManager.h"
#include "Module/RenderManager.h"

#if defined(RK_OPENGL)
    #include "OpenGL/OpenGLLoader.h"
#elif defined(RK_VULKAN)
    #include "Vulkan/VulkanLoader.h"
#elif defined(RK_SOFT_RENDER)
    #include "SoftRender/SoftRenderLoader.h"
#endif

namespace Rocket {
    class TestApplication : implements Application {
        RUNTIME_MODULE_TYPE(Application);
    public:
        virtual ~TestApplication() = default;
    };
}

#if defined(RK_OPENGL)
static Rocket::default_allocator<Rocket::OpenGLLoader> g_render_loader_allocator;
#elif defined(RK_VULKAN)
static Rocket::default_allocator<Rocket::VulkanLoader> g_render_loader_allocator;
#elif defined(RK_SOFT_RENDER)
static Rocket::default_allocator<Rocket::SoftRenderLoader> g_render_loader_allocator;
#endif

Rocket::IApplication* g_Application;
Rocket::IRuntimeModule* g_WindowManager;
Rocket::IRuntimeModule* g_RenderManager;

void AllocateModule() {
    // Allocate Application
    g_Application = RK_NEW Rocket::TestApplication;
    // Allocate Modules
    g_WindowManager = RK_NEW Rocket::WindowManager;
    g_RenderManager = RK_NEW Rocket::RenderManager;
    // Insert Graphics Drivers
#if defined(RK_OPENGL)
    static_cast<Rocket::RenderManager*>(g_RenderManager)->AddRenderLoader(std::allocate_shared<Rocket::OpenGLLoader>(g_render_loader_allocator));
#elif defined(RK_VULKAN)
    static_cast<Rocket::RenderManager*>(g_RenderManager)->AddRenderLoader(std::allocate_shared<Rocket::VulkanLoader>(g_render_loader_allocator));
#elif defined(RK_SOFT_RENDER)
    static_cast<Rocket::RenderManager*>(g_RenderManager)->AddRenderLoader(std::allocate_shared<Rocket::SoftRenderLoader>(g_render_loader_allocator));
#endif
    // Insert Modules
    g_Application->AddModule(g_WindowManager);
    g_Application->AddModule(g_RenderManager);
}

void DeallocateModule() {
    // Deallocate Modules
    RK_DELETE g_RenderManager;
    RK_DELETE g_WindowManager;
    // Deallocate Application
    RK_DELETE g_Application;
}
