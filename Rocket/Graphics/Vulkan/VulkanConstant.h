#pragma once
#include <glslang/Include/glslang_c_interface.h>
#include <vector>

namespace Rocket {
    extern const std::vector<const char*> validation_layers;
    extern const std::vector<const char*> instance_extensions;
    extern const std::vector<const char*> device_extensions;

    extern const glslang_resource_t DefaultTBuiltInResource;
}
