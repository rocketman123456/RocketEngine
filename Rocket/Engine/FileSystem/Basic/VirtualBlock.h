#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/VirtualNode.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Rocket {
    class FileSystem;
    // Store File System Info, Just like linux block and inode
    // Should Use in VirtualFileSystem
    class VirtualBlock {
    public:
        using SubBlockMap = std::unordered_map<std::string, std::shared_ptr<VirtualBlock> >;
        using NodeMap = std::unordered_map<std::string, std::shared_ptr<VirtualNode> >;
    public:
        // if file_system == null, should look to parent block
        std::shared_ptr<FileSystem> file_system = nullptr;
        // if parent == nullptr, means that this is root dir
        std::shared_ptr<VirtualBlock> parent = nullptr;
        // Parent Name to find file
        std::string block_name = "";
        // Alias Path in VFS
        std::string block_path = "";
        SubBlockMap block_map = {};
        NodeMap node_map = {};
    };

    CLASS_PTR(VirtualBlock);
}
