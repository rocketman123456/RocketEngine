#include "Common/WindowFactory.h"
#include "Vulkan/VulkanCheck.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanInstanceUtils.h"
#include "Vulkan/VulkanDeviceUtils.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

using namespace Rocket;

void DrawFrame() {

}

int main() {
    WindowInfo info;
    info.title = "Rocket";
    info.width = 1600;
    info.height = 900;

    auto window = WindowFactory::CreatePlatformWindow(info);
    window->Initialize();

    VulkanInstance instance;
    instance.window = window->GetWindowHandle();
    BL_CHECK(InitVulkanInstance(instance, instance_extensions, validation_layers));

    VulkanRenderDevice device;
    auto width = window->FramebufferWidth();
    auto height = window->FramebufferHeight();
    BL_CHECK(InitVulkanRenderDevice(instance, device, device_extensions, validation_layers, width, height));

    VulkanState state;
    //CreateDepthResources(device, info.width, info.height, state.depth_texture);

    VK_CHECK(CreateDescriptorPool(device, 1, 2, 1, &state.descriptor_pool));
    VK_CHECK(CreateDescriptorSet(device, &state, 0, 0, 0));
    VulkanRenderPassCreateInfo rp_create_info = {};
    rp_create_info.clear_color = true;
    rp_create_info.clear_depth = true;
    rp_create_info.flags = eRenderPassBit_First|eRenderPassBit_Last;
    VK_CHECK(CreateColorAndDepthRenderPass(device, true, &state.render_pass, rp_create_info));
    VK_CHECK(CreatePipelineLayout(device.device, device.table, state.descriptor_set_layout, &state.pipeline_layout));
	BL_CHECK(CreateGraphicsPipeline(device, state.render_pass, state.pipeline_layout, { "/Asset/Shader/shader_base.vert", "/Asset/Shader/shader_base.frag" }, &state.graphics_pipeline));
    // createColorAndDepthFramebuffers(vkDev, vkState.renderPass, vkState.depthTexture.imageView, vkState.swapchainFramebuffers);

    while(window->IsRunning()) {
        window->Tick();
        DrawFrame();
    }

    // Cleanup Vulkan States
    CleanupVulkanRenderDevice(device);
    CleanupVulkanInstance(instance);

    window->Finalize();
    return 0;
}
