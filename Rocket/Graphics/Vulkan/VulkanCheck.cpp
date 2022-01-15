#include "Vulkan/VulkanCheck.h"
#include "Log/Log.h"

void CHECK(bool check, const char* fileName, int lineNumber) {
    if (!check) {
        RK_ERROR(Graphics, "CHECK() failed at {}:{}", fileName, lineNumber);
        assert(false);
        exit(EXIT_FAILURE);
    }
}
