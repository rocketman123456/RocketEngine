#include "Vulkan/VulkanShaderUtils.h"
#include "Vulkan/VulkanConstant.h"
#include "Parser/ShaderParser.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

namespace Rocket {
    VkShaderStageFlagBits ShaderStageToVulkan(shaderc_shader_kind sh) {
        switch(sh) {
            case shaderc_vertex_shader:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case shaderc_fragment_shader:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            case shaderc_geometry_shader:
                return VK_SHADER_STAGE_GEOMETRY_BIT;
            case shaderc_tess_control_shader:
                return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            case shaderc_tess_evaluation_shader:
                return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            case shaderc_compute_shader:
                return VK_SHADER_STAGE_COMPUTE_BIT;
        }
        return VK_SHADER_STAGE_VERTEX_BIT;
    }

    shaderc_shader_kind ShaderStageFromFileName(const std::string& fileName) {
        if (EndsWith(fileName, ".vert"))
            return shaderc_vertex_shader;
        if (EndsWith(fileName, ".frag"))
            return shaderc_fragment_shader;
        if (EndsWith(fileName, ".geom"))
            return shaderc_geometry_shader;
        if (EndsWith(fileName, ".comp"))
            return shaderc_compute_shader;
        if (EndsWith(fileName, ".tesc"))
            return shaderc_tess_control_shader;
        if (EndsWith(fileName, ".tese"))
            return shaderc_tess_evaluation_shader;
        return shaderc_vertex_shader;
    }

    std::string PreProcessShader(
        shaderc_shader_kind kind,
        const std::string& source_name,
        const std::string& source
    ) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        // Like -DRK_DEFINE=1
        options.AddMacroDefinition("RK_DEFINE", "1");

        shaderc::PreprocessedSourceCompilationResult result =
            compiler.PreprocessGlsl(source, kind, source_name.c_str(), options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            RK_ERROR(Graphics, "GLSL preprocessing failed: {}", result.GetErrorMessage());
            return "";
        }

        return { result.cbegin(), result.cend() };
    }

    std::string CompileToAssembly(
        shaderc_shader_kind kind,
        const std::string& source_name,
        const std::string& source,
        bool optimize
    ) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        // Like -DRK_DEFINE=1
        options.AddMacroDefinition("RK_DEFINE", "1");
        if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(
            source, kind, source_name.c_str(), options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            RK_ERROR(Graphics, "GLSL compile to assembly failed: {}", result.GetErrorMessage());
            return "";
        }

        return {result.cbegin(), result.cend()};
    }

    std::vector<uint32_t> CompileToBinary(
        shaderc_shader_kind kind,
        const std::string& source_name,
        const std::string& source,
        bool optimize
    ) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        // Like -DRK_DEFINE=1
        options.AddMacroDefinition("RK_DEFINE", "1");
        if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::SpvCompilationResult module =
            compiler.CompileGlslToSpv(source, kind, source_name.c_str(), options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            RK_ERROR(Graphics, "GLSL compile to binary failed: {}", module.GetErrorMessage());
            return std::vector<uint32_t>();
        }

        return {module.cbegin(), module.cend()};
    }

    std::size_t CompileShaderFile(
        const std::string& root, const std::string& file, VulkanShaderModule* module
    ) {
        auto source = ReadShaderFile(root, file);
        if(!source.empty()) {
            auto kind = ShaderStageFromFileName(file);
            source = PreProcessShader(kind, file, source);
            auto assembly = CompileToAssembly(kind, file, source);
            module->SPIRV = CompileToBinary(kind, file, assembly);
            return module->SPIRV.size();
        }
        return 0;
    }

    VkResult CreateShaderModule(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const std::string& root, 
        const std::string& fileName,
        VulkanShaderModule* shader
    ) {
        RK_INFO(Graphics, "shader: {}", root + fileName);
        if (CompileShaderFile(root, fileName, shader) < 1)
            return VK_NOT_READY;
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shader->SPIRV.size() * sizeof(unsigned int);
        createInfo.pCode = shader->SPIRV.data();
        return table.vkCreateShaderModule(device, &createInfo, nullptr, &shader->shader_module);
    }
}
