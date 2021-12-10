#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/FileSystem.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Rocket {
    class VirtualFileSystem;
    // Store File System Info, Just like linux block and inode
    // Should Use in VirtualFileSystem
    class VirtualBlock {
        friend class VirtualFileSystem;
    public:
        using SubBlockMap = std::unordered_map<std::string, std::shared_ptr<VirtualBlock> >;
        using NodeMap = std::unordered_map<std::string, std::shared_ptr<VirtualNode> >;
    public:
        // if file_system == null, should look to parent block
        FileSystemPtr file_system = nullptr;
        // if parent == nullptr, means that this is root dir
        std::shared_ptr<VirtualBlock> parent = nullptr;
        std::string block_name = "";
        std::string block_path = "";
        SubBlockMap block_map = {};
        NodeMap node_map = {};
    };

    CLASS_PTR(VirtualBlock);
}
