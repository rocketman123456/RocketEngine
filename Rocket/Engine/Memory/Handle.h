#pragma once
#include "Core/Core.h"
#include "Core/Template.h"

namespace Rocket {
    // Use Handle for Memory Manage and hide real pointer
    typedef struct Handle {
        uint32_t index;     // index for memory pool
        uint16_t category;  // hash of category name
        uint16_t type;      // hash of type name
        uint8_t pattern;    // hash of content
    } Handle;
}
