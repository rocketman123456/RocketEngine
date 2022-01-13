#include "Vulkan/VulkanVariable.h"

#include <set>

namespace Rocket {
    bool QueueFamilyIndices::IsComplete() {
        return 
            graphics_family.has_value() && 
            present_family.has_value() && 
            compute_family.has_value();
    }

    uint32_t QueueFamilyIndices::Multiplicity() {
        if(!multiplicity.has_value()) {
            std::set<uint32_t> families = {
                graphics_family.value(),
                present_family.value(),
                compute_family.value(),
            };
            multiplicity = families.size();
        }
        return multiplicity.value();
    }

    const std::vector<uint32_t>& QueueFamilyIndices::FamilyData() {
        if(!family_data.has_value()) {
            std::set<uint32_t> families = {
                graphics_family.value(),
                present_family.value(),
                compute_family.value(),
            };
            std::vector<uint32_t> data = {};
            data.insert(data.begin(), families.begin(), families.end());
            family_data = data;
        }
        return family_data.value();
    }
}
