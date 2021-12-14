#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/VirtualNode.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

#include <unordered_map>
#include <memory>
#include <string>

namespace Rocket {
    class FileSystem;
    // Store File System Info, Just like linux block and inode
    // Should Use in VirtualFileSystem
    struct VirtualBlock {
        using SubBlockMap = std::unordered_map<std::string, std::shared_ptr<VirtualBlock> >;
        using NodeMap = std::unordered_map<std::string, std::shared_ptr<VirtualNode> >;

        // if file_system == null, should look to parent block
        std::shared_ptr<FileSystem> file_system = nullptr;
        // if parent == nullptr, means that this is root dir
        std::shared_ptr<VirtualBlock> parent = nullptr;
        // Parent Name to find file
        std::string name = "";
        // Alias Path in VFS
        std::string path = "";
        // Store data
        SubBlockMap block_map = {};
        NodeMap node_map = {};
    };

    CLASS_PTR(VirtualBlock);

    static VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
    static VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
    static VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
    static VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    static VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, const std::string& name);

    static VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        // check invalid status
        if(dirs.size() == 0) return root;
        if(root == nullptr) return nullptr;
        // find if dir exist
        auto found = root->block_map.find(dirs[level]);
        // if not exist, return null
        if(found == root->block_map.end()) { return nullptr; }
        // check return or go deeper
        VirtualBlockPtr sub_block = found->second;
        level = level + 1;
        if(level == dirs.size()) return sub_block;
        else return FindVirtualBlock(sub_block, dirs, level);
    }

    static VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs) {
        return FindVirtualBlock(root, dirs, 0);
    }

    static VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        // check invalid status
        if(dirs.size() == 0) return root;
        if(root == nullptr) return nullptr;
        // find if dir exist
        auto found = root->block_map.find(dirs[level]);
        // we reach deepest dir
        if(found == root->block_map.end()) { return root; }
        // check return or go deeper
        VirtualBlockPtr sub_block = found->second;
        level = level + 1;
        if(level == dirs.size()) return sub_block;
        else return FindVirtualBlock(sub_block, dirs, level);
    }

    static VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs) {
        return FindDeepestExistVirtualBlock(root, dirs, 0);
    }

    static VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, const std::string& name) {
        // find file parent block
        auto block = FindVirtualBlock(root, dirs, 0);
        if(block == nullptr) return nullptr;
        // find target file
        auto found = root->node_map.find(name);
        if(found == root->node_map.end()) { return nullptr; }
        return found->second;
    }
}
