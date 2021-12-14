#pragma once
#include "Core/MemoryDefine.h"
#include "Utils/StringUtils.h"
#include "Utils/Hashing.h"
#include "Log/Log.h"

#include <memory>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

namespace Rocket {
    class VirtualBlock;
    // Contain Basic and Additional File Infos
    // Can be modified if needed
    struct VirtualNode {
        std::vector<std::string> stack = {};
        std::string path = "";  // file basic path in vfs
        std::string name = "";  // file name
        std::shared_ptr<VirtualBlock> vblock = nullptr;
        std::shared_ptr<VirtualNode> vlink = nullptr;
    };

    CLASS_PTR(VirtualNode);
}
