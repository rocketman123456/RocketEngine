#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

const VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "Vulkan SDL tutorial",
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "No Engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_0,
};

const VkInstanceCreateInfo createInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledExtensionCount = 2,
    .ppEnabledExtensionNames = (const char* const[]) { VK_KHR_SURFACE_EXTENSION_NAME },
};

#ifdef VULKAN_ENABLE_LUNARG_VALIDATION
const VkInstanceCreateInfo createInfoLunarGValidation = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledLayerCount = 1,
    .ppEnabledLayerNames = (const char* const[]) { "VK_LAYER_LUNARG_standard_validation" },
    .enabledExtensionCount = 3,
    .ppEnabledExtensionNames = (const char* const[]) { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME },
};

PFN_vkCreateDebugReportCallbackEXT vkpfn_CreateDebugReportCallbackEXT = 0;
PFN_vkDestroyDebugReportCallbackEXT vkpfn_DestroyDebugReportCallbackEXT = 0;

VkDebugReportCallbackEXT debugCallback = VK_NULL_HANDLE;

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanReportFunc(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData)
{
    printf("VULKAN VALIDATION: %s\n", msg);
    return VK_FALSE;
}

VkDebugReportCallbackCreateInfoEXT debugCallbackCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
    .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
    .pfnCallback = VulkanReportFunc,
};
#endif

VkInstance instance = VK_NULL_HANDLE;

int init_vulkan()
{
    VkResult result = VK_ERROR_INITIALIZATION_FAILED;
#ifdef VULKAN_ENABLE_LUNARG_VALIDATION
    result = vkCreateInstance(&createInfoLunarGValidation, 0, &instance);
    if (result == VK_SUCCESS)
    {
        vkpfn_CreateDebugReportCallbackEXT =
            (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
        vkpfn_DestroyDebugReportCallbackEXT =
            (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
        if (vkpfn_CreateDebugReportCallbackEXT && vkpfn_DestroyDebugReportCallbackEXT)
        {
            vkpfn_CreateDebugReportCallbackEXT(instance, &debugCallbackCreateInfo, 0, &debugCallback);
        }
    }
#endif
    if (result != VK_SUCCESS)
    {
        result = vkCreateInstance(&createInfo, 0, &instance);
    }

    return result == VK_SUCCESS;
}

void fini_vulkan()
{
#ifdef VULKAN_ENABLE_LUNARG_VALIDATION
    if (vkpfn_DestroyDebugReportCallbackEXT && debugCallback)
    {
        vkpfn_DestroyDebugReportCallbackEXT(instance, debugCallback, 0);
    }
#endif
    vkDestroyInstance(instance, 0);
}

SDL_Window* window = 0;
//VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
VkSurfaceKHR surface = VK_NULL_HANDLE;
uint32_t width = 1280;
uint32_t height = 720;

int init_window()
{
    window = SDL_CreateWindow(
        "Vulkan Sample",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        0
    );

    //SDL_SysWMinfo info;
    //SDL_VERSION(&info.version);
    //SDL_GetWindowWMInfo(window, &info);

    //surfaceCreateInfo.hinstance = GetModuleHandle(0);
    //surfaceCreateInfo.hwnd = info.info.win.window;

    SDL_Vulkan_CreateSurface(window, instance, &surface);
    //VkResult result = vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface);

    return window != 0;// && result == VK_SUCCESS;
}

void fini_window()
{
    vkDestroySurfaceKHR(instance, surface, 0);
    SDL_DestroyWindow(window);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    [[maybe_unused]]SDL_Window* window = SDL_CreateWindow(
        "Rocket",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        SDL_WINDOW_VULKAN
    );

    init_vulkan();
    //init_window();
    //init_device();
    //init_swapchain();
    //init_render();

    SDL_Event window_event;
    while(1) {
        if (SDL_PollEvent(&window_event)) {
            if (window_event.type == SDL_QUIT) {
                break;
            }
        }

        //draw_frame();
    }

    //fini_render();
    //fini_swapchain();
    //fini_device();
    //fini_window();
    fini_vulkan();

    SDL_Quit();
    return 0;
}