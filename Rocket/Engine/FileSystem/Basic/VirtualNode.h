#pragma once
#include "Core/MemoryDefine.h"
#include "Utils/StringUtils.h"
#include "Utils/Hashing.h"
#include "Log/Log.h"

#include <memory>
#include <string>
#include <exception>
#include <stdexcept>

namespace Rocket {
    class VirtualBlock;
    // Contain Basic and Additional File Infos
    // Can be modified if needed
    struct VirtualNode {
        // std::vector<std::string> stack = {};
        // File Full Name will be path + name
        std::string path = "";  // file basic path in vfs
        std::string name = "";  // file name
        std::shared_ptr<VirtualBlock> vblock = nullptr;
        // int32_t crc = 0;
        // TODO : add support for link function
        // std::shared_ptr<VirtualNode> vlink = nullptr;
    };

    CLASS_PTR(VirtualNode);
}
