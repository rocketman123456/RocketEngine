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
        // Store data
        SubBlockMap block_map = {};
        NodeMap node_map = {};
    };

    CLASS_PTR(VirtualBlock);

    static VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        // check invalid status
        if(dirs.size() == 0) return root;
        if(root == nullptr) return nullptr;
        // find if dir exist
        auto found = root->block_map.find(dirs[level]);
        if(found == root->block_map.end()) { return nullptr; }
        // check return or go deeper
        VirtualBlockPtr sub_block = found->second;
        level = level + 1;
        if(level == dirs.size())
            return sub_block;
        else
            return FindVirtualBlock(sub_block, dirs, level);
    }

    static VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs) {
        return FindVirtualBlock(root, dirs, 0);
    }

    static VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::string& file_name) {
        std::vector<std::string> path_stack;
        std::string path = file_name;
        Replace(path, "\\", "/");
        SplitSingleChar(path, path_stack, '/');
        return FindVirtualBlock(root, path_stack, 0);
    }

    static VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& file_path, const std::string& file_name) {
        // split file path and file name
        std::vector<std::string> path_stack;
        SplitSingleChar(file_path, path_stack, '/');
        auto block = FindVirtualBlock(root, path_stack);
        if(block == nullptr) return nullptr;
        auto node = block->node_map.find(file_name);
        if(node == block->node_map.end()) return nullptr;
        return node->second;
    }

    static VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& file_name) {
        // split file path and file name
        std::string path = file_name;
        Replace(path, "\\", "/");
        std::string first = "";
        std::string second = "";
        SplitLastSingleChar(path, first, second, '/');
        //RK_TRACE(File, "File Path: {}, File Name: {}", first, second);
        return FindVirtualNode(root, first, second);
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
        if(level == dirs.size())
            return sub_block;
        else
            return FindVirtualBlock(sub_block, dirs, level);
    }
}
