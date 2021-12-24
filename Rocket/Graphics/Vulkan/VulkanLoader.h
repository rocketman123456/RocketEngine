#pragma once
#include "Common/RenderLoader.h"

namespace Rocket {
    class VulkanLoader : _implements_ RenderLoader {
    public:
        VulkanLoader() = default;
        virtual ~VulkanLoader() = default;
        void Load() final;
        void Unload() final;
    };
}
