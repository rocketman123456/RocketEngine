#include "Common/WindowFactory.h"
#include "Vulkan/VulkanCheck.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanInstanceUtils.h"
#include "Vulkan/VulkanDeviceUtils.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

using namespace Rocket;

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

    // createDescriptorPool(vkDev, 1, 2, 1, &vkState.descriptorPool);
    // createDescriptorSet();
    // VulkanRenderPassCreateInfo rp_create_info = {};
    // rp_create_info.clearColor = true;
    // rp_create_info.clearDepth = true;
    // rp_create_info.flags = eRenderPassBit_First|eRenderPassBit_Last;
    // createColorAndDepthRenderPass(vkDev, true, &vkState.renderPass, rp_create_info);
    // createPipelineLayout(vkDev.device, vkState.descriptorSetLayout, &vkState.pipelineLayout);
	// createGraphicsPipeline(vkDev, vkState.renderPass, vkState.pipelineLayout, { "data/shaders/chapter03/VK02.vert", "data/shaders/chapter03/VK02.frag", "data/shaders/chapter03/VK02.geom" }, &vkState.graphicsPipeline));
    // createColorAndDepthFramebuffers(vkDev, vkState.renderPass, vkState.depthTexture.imageView, vkState.swapchainFramebuffers);

    while(window->IsRunning()) {
        window->Tick();
    }

    CleanupVulkanRenderDevice(device);
    CleanupVulkanInstance(instance);

    window->Finalize();
    return 0;
}
