#include "Vulkan/VulkanVariable.h"

#include <set>

namespace Rocket {
    bool QueueFamilyIndices::IsComplete() {
        PostProcess();
        return graphics_family.has_value() && 
            present_family.has_value() && 
            compute_family.has_value() &&
            multiplicity.has_value();
    }

    void QueueFamilyIndices::PostProcess() {
        if(!multiplicity.has_value()) {
            std::set<uint32_t> families = {
                graphics_family.value(),
                present_family.value(),
                compute_family.value(),
            };
            multiplicity = families.size();
            std::vector<uint32_t> data = {};
            data.insert(data.begin(), families.begin(), families.end());
            unique_family_data = data;
        }
    }
}
