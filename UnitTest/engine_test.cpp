#include "Core/Core.h"
#include "Module/Application.h"
#include "Module/WindowManager.h"
#include "Module/EventManager.h"
#include "Module/RenderManager.h"

#if defined(RK_OPENGL)
    #include "OpenGL/OpenGLLoader.h"
#elif defined(RK_VULKAN)
    #include "Vulkan/VulkanLoader.h"
#elif defined(RK_SOFT_RENDER)
    #include "SoftRender/SoftRenderLoader.h"
#elif defined(RK_METAL)

#endif

namespace Rocket {
    IApplication* g_Application;
    IRuntimeModule* g_WindowManager;
    IRuntimeModule* g_RenderManager;
    IRuntimeModule* g_EventManager;

    class TestApplication : implements Application {
        RUNTIME_MODULE_TYPE(Application);
    public:
        virtual ~TestApplication() = default;

        void PostInitializeModule() final {
            RK_TRACE("TestApplication::PostInitializeModule");
            //REGISTER_DELEGATE_CLASS(TestApplication, TestApplication::OnEvent, this, window_close);
            //static_cast<EventManager*>(g_EventManager)->AddListener();
            bool ret = false;
            EventListenerDelegate delegate;
            delegate.Bind<IApplication, &IApplication::OnCloseEvent>(g_Application);
            ret = static_cast<EventManager*>(g_EventManager)->AddListener(delegate, "window_close"_hash);
        }
    };
}

#if defined(RK_OPENGL)
static Rocket::default_allocator<Rocket::OpenGLLoader> g_render_loader_allocator;
#elif defined(RK_VULKAN)
static Rocket::default_allocator<Rocket::VulkanLoader> g_render_loader_allocator;
#elif defined(RK_SOFT_RENDER)
static Rocket::default_allocator<Rocket::SoftRenderLoader> g_render_loader_allocator;
#elif defined(RK_METAL)
// TODO: add metal loader
#endif

using namespace Rocket;

void AllocateModule() {
    // Allocate Application
    g_Application = new Rocket::TestApplication;
    // Allocate Modules
    g_WindowManager = new Rocket::WindowManager;
    g_EventManager = new Rocket::EventManager;
    g_RenderManager = new Rocket::RenderManager;
    // Insert Graphics Drivers
#if defined(RK_OPENGL)
    static_cast<Rocket::RenderManager*>(g_RenderManager)->AddRenderLoader(std::allocate_shared<Rocket::OpenGLLoader>(g_render_loader_allocator));
#elif defined(RK_VULKAN)
    static_cast<Rocket::RenderManager*>(g_RenderManager)->AddRenderLoader(std::allocate_shared<Rocket::VulkanLoader>(g_render_loader_allocator));
#elif defined(RK_SOFT_RENDER)
    static_cast<Rocket::RenderManager*>(g_RenderManager)->AddRenderLoader(std::allocate_shared<Rocket::SoftRenderLoader>(g_render_loader_allocator));
#elif defined(RK_METAL)

#endif
    // Insert Modules
    g_Application->AddModule(g_WindowManager);
    g_Application->AddModule(g_EventManager);
    g_Application->AddModule(g_RenderManager);
}

void DeallocateModule() {
    // Deallocate Modules
    delete g_RenderManager;
    delete g_EventManager;
    delete g_WindowManager;
    // Deallocate Application
    delete g_Application;
}
