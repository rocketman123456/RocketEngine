#include "Vulkan/VulkanInstanceUtils.h"
#include "Vulkan/VulkanConstant.h"
#include "Parser/ShaderParser.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

#include <glslang/Include/ResourceLimits.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT             messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void*                                       pUserData
) {
    if(messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        return VK_FALSE;
    } else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        RK_WARN(Graphics, "validation layer: {}", pCallbackData->pMessage);
        return VK_FALSE;
    } else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        RK_ERROR(Graphics, "validation layer: {}", pCallbackData->pMessage);
        return VK_FALSE;
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(
    VkDebugReportFlagsEXT       flags,
    VkDebugReportObjectTypeEXT  objectType,
    uint64_t                    object,
    size_t                      location,
    int32_t                     messageCode,
    const char*                 pLayerPrefix,
    const char*                 pMessage,
    void*                       UserData
) {
    // https://github.com/zeux/niagara/blob/master/src/device.cpp  
    // [ignoring performance warnings]
    // This silences warnings like "For optimal performance image layout 
    // should be VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL instead of GENERAL."
    if(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        RK_WARN(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        RK_ERROR(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        RK_INFO(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
        return VK_FALSE;
    }
}

namespace Rocket {
    void CHECK(bool check, const char* fileName, int lineNumber) {
        if (!check) {
            RK_ERROR(Graphics, "CHECK() failed at {}:{}", fileName, lineNumber);
            assert(false);
            exit(EXIT_FAILURE);
        }
    }

    void PrintVulkanVersion() {
        auto version = volkGetInstanceVersion();
        RK_INFO(Graphics ,"Vulkan version {}.{}.{} initialized.",
            VK_VERSION_MAJOR(version),
            VK_VERSION_MINOR(version),
            VK_VERSION_PATCH(version)
        );
    }

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = 
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = 
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = VulkanDebugCallback;
    }

    void PopulateDebugReportCreateInfo(VkDebugReportCallbackCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.pNext = nullptr;
        createInfo.flags =
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT;
        createInfo.pfnCallback = &VulkanDebugReportCallback;
        createInfo.pUserData = nullptr;
    }

    void SetupDebugMessenger(
        const VkInstance& instance, VkDebugUtilsMessengerEXT* messenger
    ) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateDebugMessengerCreateInfo(createInfo);
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, messenger));
    }

    void SetupDebugReportCallback(
        const VkInstance& instance, VkDebugReportCallbackEXT* reportCallback
    ) {
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        PopulateDebugReportCreateInfo(createInfo);
        VK_CHECK(vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, reportCallback));
    }

    static_assert(sizeof(TBuiltInResource) == sizeof(glslang_resource_t));

    VkShaderStageFlagBits glslangShaderStageToVulkan(glslang_stage_t sh) {
        switch(sh) {
            case GLSLANG_STAGE_VERTEX:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case GLSLANG_STAGE_FRAGMENT:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            case GLSLANG_STAGE_GEOMETRY:
                return VK_SHADER_STAGE_GEOMETRY_BIT;
            case GLSLANG_STAGE_TESSCONTROL:
                return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            case GLSLANG_STAGE_TESSEVALUATION:
                return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            case GLSLANG_STAGE_COMPUTE:
                return VK_SHADER_STAGE_COMPUTE_BIT;
        }
        return VK_SHADER_STAGE_VERTEX_BIT;
    }

    glslang_stage_t glslangShaderStageFromFileName(const char* fileName) {
        if (EndsWith(fileName, ".vert"))
            return GLSLANG_STAGE_VERTEX;
        if (EndsWith(fileName, ".frag"))
            return GLSLANG_STAGE_FRAGMENT;
        if (EndsWith(fileName, ".geom"))
            return GLSLANG_STAGE_GEOMETRY;
        if (EndsWith(fileName, ".comp"))
            return GLSLANG_STAGE_COMPUTE;
        if (EndsWith(fileName, ".tesc"))
            return GLSLANG_STAGE_TESSCONTROL;
        if (EndsWith(fileName, ".tese"))
            return GLSLANG_STAGE_TESSEVALUATION;
        return GLSLANG_STAGE_VERTEX;
    }

    size_t CompileShader(
        glslang_stage_t stage, const char* shaderSource, VulkanShaderModule* shaderModule
    ) {
        glslang_input_t input = {};
        input.language = GLSLANG_SOURCE_GLSL;
        input.stage = stage;
        input.client = GLSLANG_CLIENT_VULKAN;
        input.client_version = GLSLANG_TARGET_VULKAN_1_1;
        input.target_language = GLSLANG_TARGET_SPV;
        input.target_language_version = GLSLANG_TARGET_SPV_1_3;
        input.code = shaderSource;
        input.default_version = 100;
        input.default_profile = GLSLANG_NO_PROFILE;
        input.force_default_version_and_profile = false;
        input.forward_compatible = false;
        input.messages = GLSLANG_MSG_DEFAULT_BIT;
        input.resource = (const glslang_resource_t*) &DefaultTBuiltInResource;

        glslang_shader_t* shader = glslang_shader_create(&input);

        if (!glslang_shader_preprocess(shader, &input)) {
            RK_ERROR(Graphics, "GLSL preprocessing failed");
            RK_ERROR(Graphics, "{}", glslang_shader_get_info_log(shader));
            RK_ERROR(Graphics, "{}", glslang_shader_get_info_debug_log(shader));
            PrintShaderSource(input.code);
            return 0;
        }

        if (!glslang_shader_parse(shader, &input)) {
            RK_ERROR(Graphics, "GLSL parsing failed");
            RK_ERROR(Graphics, "{}", glslang_shader_get_info_log(shader));
            RK_ERROR(Graphics, "{}", glslang_shader_get_info_debug_log(shader));
            PrintShaderSource(glslang_shader_get_preprocessed_code(shader));
            return 0;
        }

        glslang_program_t* program = glslang_program_create();
        glslang_program_add_shader(program, shader);

        if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT)) {
            RK_ERROR(Graphics, "GLSL linking failed");
            RK_ERROR(Graphics, "{}", glslang_program_get_info_log(program));
            RK_ERROR(Graphics, "{}", glslang_program_get_info_debug_log(program));
            return 0;
        }

        glslang_program_SPIRV_generate(program, stage);
        shaderModule->SPIRV.resize(glslang_program_SPIRV_get_size(program));
        glslang_program_SPIRV_get(program, shaderModule->SPIRV.data());

        const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
        if (spirv_messages) {
            RK_ERROR(Graphics, "{}", spirv_messages);
        }

        glslang_program_delete(program);
        glslang_shader_delete(shader);
        return shaderModule->SPIRV.size();
    }

    size_t CompileShaderFile(
        const char* root, const char* file, VulkanShaderModule* shaderModule
    ) {
        auto shaderSource = ReadShaderFile(root, file);
        if(!shaderSource.empty())
            return CompileShader(
                glslangShaderStageFromFileName(file), shaderSource.c_str(), shaderModule);
        return 0;
    }

    VkResult CreateShaderModule(
        VkDevice device, VulkanShaderModule* shader, const char* root, const char* fileName
    ) {
        if (CompileShaderFile(root, fileName, shader) < 1)
            return VK_NOT_READY;
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shader->SPIRV.size() * sizeof(unsigned int);
        createInfo.pCode = shader->SPIRV.data();
        return vkCreateShaderModule(device, &createInfo, nullptr, &shader->shader_module);
    }

    uint32_t FindQueueFamilies(VkPhysicalDevice device, VkQueueFlags desiredFlags) {
        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
        std::vector<VkQueueFamilyProperties> families(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

        for (uint32_t i = 0; i != families.size(); i++) {
            if (families[i].queueCount > 0 && families[i].queueFlags & desiredFlags) {
                return i;
            }
        }
        return 0;
    }

    uint32_t FindPresentFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                return i;
            }
            i++;
        }
        return 0;
    }

    VkPipelineShaderStageCreateInfo ShaderStageInfo(
            VkShaderStageFlagBits shaderStage, 
            const VulkanShaderModule& module, 
            const char* entryPoint) {
        VkPipelineShaderStageCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.stage = shaderStage;
        createInfo.module = module.shader_module;
        createInfo.pName = entryPoint;
        createInfo.pSpecializationInfo = nullptr;
        return createInfo;
    }

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(
            uint32_t binding, 
            VkDescriptorType descriptorType, 
            VkShaderStageFlags stageFlags, 
            uint32_t descriptorCount) {
        VkDescriptorSetLayoutBinding layout = {};
        layout.binding = binding;
        layout.descriptorType = descriptorType;
        layout.descriptorCount = descriptorCount;
        layout.stageFlags = stageFlags;
        layout.pImmutableSamplers = nullptr;
        return layout;
    }

    VkWriteDescriptorSet BufferWriteDescriptorSet(
            VkDescriptorSet ds, 
            const VkDescriptorBufferInfo* bi, 
            uint32_t bindIdx, 
            VkDescriptorType dType) {
        VkWriteDescriptorSet descriptorSet = {};
        descriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorSet.pNext = nullptr;
        descriptorSet.dstSet = ds;
        descriptorSet.dstBinding = bindIdx;
        descriptorSet.dstArrayElement = 0;
        descriptorSet.descriptorCount = 1; 
        descriptorSet.descriptorType = dType; 
        descriptorSet.pImageInfo = nullptr;
        descriptorSet.pBufferInfo = bi;
        descriptorSet.pTexelBufferView = nullptr;
        return descriptorSet;
    }

    VkWriteDescriptorSet ImageWriteDescriptorSet(
            VkDescriptorSet ds, 
            const VkDescriptorImageInfo* ii, 
            uint32_t bindIdx) {
        VkWriteDescriptorSet descriptorSet = {};
        descriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorSet.pNext = nullptr;
        descriptorSet.dstSet = ds;
        descriptorSet.dstBinding = bindIdx;
        descriptorSet.dstArrayElement = 0;
        descriptorSet.descriptorCount = 1; 
        descriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
        descriptorSet.pImageInfo = ii;
        descriptorSet.pBufferInfo = nullptr;
        descriptorSet.pTexelBufferView = nullptr;
        return descriptorSet;
    }

    uint32_t GetVulkanBufferAlignment(VulkanRenderDevice& vkDev) {
        VkPhysicalDeviceProperties devProps;
        vkGetPhysicalDeviceProperties(vkDev.physical_device, &devProps);
        return static_cast<uint32_t>(devProps.limits.minStorageBufferOffsetAlignment);
    }

    // Check if the texture is used as a depth buffer
    bool IsDepthFormat(VkFormat fmt) {
        return
            (fmt == VK_FORMAT_D16_UNORM) ||
            (fmt == VK_FORMAT_X8_D24_UNORM_PACK32) ||
            (fmt == VK_FORMAT_D32_SFLOAT) ||
            (fmt == VK_FORMAT_D16_UNORM_S8_UINT) ||
            (fmt == VK_FORMAT_D24_UNORM_S8_UINT) ||
            (fmt == VK_FORMAT_D32_SFLOAT_S8_UINT);
    }

    bool SetVkImageName(VulkanRenderDevice& vkDev, void* object, const char* name) {
        return SetVkObjectName(vkDev, object, VK_OBJECT_TYPE_IMAGE, name);
    }
}
