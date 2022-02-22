#include "Common/WindowFactory.h"
#include "Vulkan/VulkanCheck.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanInstanceUtils.h"
#include "Vulkan/VulkanDeviceUtils.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>
#include <volk.h>

using namespace Rocket;

VulkanInstance instance;
VulkanRenderDevice device;
VulkanState state;

uint32_t currentFrame = 0;

static constexpr VkClearColorValue clearValueColor = { 0.2f, 0.3f, 0.3f, 1.0f };

bool FillCommandBuffers(size_t i) {
	VkCommandBufferBeginInfo bi = {};
	bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	bi.pNext = nullptr;
	bi.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	bi.pInheritanceInfo = nullptr;

    VkClearValue color = {};
    color.color = clearValueColor;
	const std::array<VkClearValue, 2> clearValues = { color };

	VkRect2D screenRect = {};
	screenRect.offset = { 0, 0 },
	screenRect.extent.width = device.framebuffer_width;
    screenRect.extent.height = device.framebuffer_height;

	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.pNext = nullptr;
	renderPassInfo.renderPass = state.render_pass;
	renderPassInfo.framebuffer = state.swapchain_framebuffers[i];
	renderPassInfo.renderArea = screenRect;
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

    VK_CHECK(device.table.vkBeginCommandBuffer(device.command_buffers[i], &bi));

	device.table.vkCmdBeginRenderPass(device.command_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	device.table.vkCmdBindPipeline(device.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, state.graphics_pipeline);
	device.table.vkCmdBindDescriptorSets(device.command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, state.pipeline_layout, 0, 1, &state.descriptor_sets[i], 0, nullptr);
	device.table.vkCmdDraw(device.command_buffers[i], 0, 1, 0, 0);
    //device.table.vkCmdDraw(device.commandBuffers[i], static_cast<uint32_t>(indexBufferSize / (sizeof(unsigned int))), 1, 0, 0);
	device.table.vkCmdEndRenderPass(device.command_buffers[i]);

	VK_CHECK(device.table.vkEndCommandBuffer(device.command_buffers[i]));
	return true;
}

void DrawFrame() {
    device.table.vkWaitForFences(device.device, 1, &device.in_flight_fences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    device.table.vkAcquireNextImageKHR(device.device, device.swapchain, UINT64_MAX, device.image_available_semaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (device.images_in_flight[imageIndex] != VK_NULL_HANDLE) {
        device.table.vkWaitForFences(device.device, 1, &device.images_in_flight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    device.images_in_flight[imageIndex] = device.in_flight_fences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {device.image_available_semaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &device.command_buffers[imageIndex];

    VkSemaphore signalSemaphores[] = {device.render_finish_semaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    device.table.vkResetFences(device.device, 1, &device.in_flight_fences[currentFrame]);

    FillCommandBuffers(currentFrame);

    if (device.table.vkQueueSubmit(device.graphics_queue, 1, &submitInfo, device.in_flight_fences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {device.swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    device.table.vkQueuePresentKHR(device.present_queue, &presentInfo);

    auto MAX_FRAMES_IN_FLIGHT = device.swapchain_images.size();
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

int main() {
    WindowInfo info;
    info.title = "Rocket";
    info.width = 1600;
    info.height = 900;

    auto window = WindowFactory::CreatePlatformWindow(info);
    window->Initialize();

    instance.window = window->GetWindowHandle();
    VK_CHECK(InitVulkanInstance(instance, instance_extensions, validation_layers));

    auto width = window->FramebufferWidth();
    auto height = window->FramebufferHeight();
    VK_CHECK(InitVulkanRenderDevice(instance, device, device_extensions, validation_layers, width, height));

    VK_CHECK(CreateDepthResources(device, info.width, info.height, state.depth_texture));
    VK_CHECK(CreateDescriptorPool(device, 1, 2, 1, &state.descriptor_pool));
    VK_CHECK(CreateDescriptorSet(device, &state, 0, 0, 0));
    VulkanRenderPassCreateInfo rp_create_info = {};
    rp_create_info.clear_color = true;
    rp_create_info.clear_depth = true;
    rp_create_info.flags = eRenderPassBit_First|eRenderPassBit_Last;
    VK_CHECK(CreateColorAndDepthRenderPass(device, true, &state.render_pass, rp_create_info));
    VK_CHECK(CreatePipelineLayout(device.device, device.table, state.descriptor_set_layout, &state.pipeline_layout));
	VK_CHECK(CreateGraphicsPipeline(device, state.render_pass, state.pipeline_layout, { "/Asset/Shader/shader_base.vert", "/Asset/Shader/shader_base.frag" }, &state.graphics_pipeline));
    VK_CHECK(CreateColorAndDepthFramebuffers(device, state.render_pass, state.depth_texture.image_view, state.swapchain_framebuffers));

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
