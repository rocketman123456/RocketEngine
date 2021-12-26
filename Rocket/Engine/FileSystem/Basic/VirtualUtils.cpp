#include "FileSystem/Basic/VirtualUtils.h"
#include "FileSystem/Basic/FileSystem.h"

#include <cassert>

namespace Rocket {
    std::string GetVirtualPath(const VirtualNodePtr& vnode) {
        assert(vnode != nullptr);
        return vnode->path + vnode->name;
    }

    std::string GetRealPath(const VirtualNodePtr& vnode) {
        assert(vnode != nullptr);
        auto& fs_real_path = vnode->vblock->file_system->RealPath();
        auto& fs_virtual_path = vnode->vblock->file_system->VirtualPath();
        return fs_real_path + vnode->path.substr(fs_virtual_path.size()) + vnode->name;
    }

    VirtualBlockPtr CreateVirtualBlock(VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        if(root == nullptr) return nullptr;
        if(dirs.size() == 0 || level == dirs.size()) return root;
        auto found = root->block_map.find(dirs[level]);
        if(found == root->block_map.end()) {
            VirtualBlockPtr block = std::make_shared<VirtualBlock>();
            block->parent = root;
            block->name = dirs[level];
            block->path = root->path + block->name + "/";
            root->block_map[block->name] = block;
            //block_map[block->path] = block;
            RK_TRACE(File, "Block Path: {}", block->path);
            return CreateVirtualBlock(block, dirs, level + 1);
        } else {
            return CreateVirtualBlock(found->second, dirs, level + 1);
        }
    }

    VirtualBlockPtr CreateVirtualBlock(VirtualBlockPtr& root, const std::string& path) {
        auto dir_ = Replace(path, "\\", "/");
        std::vector<std::string> dir_stack;
        SplitSingleChar(dir_, &dir_stack, '/');
        auto final_block = CreateVirtualBlock(root, dir_stack, 0);
        return final_block;
    }

    VirtualNodePtr CreateVirtualNode(VirtualBlockPtr& root, const std::string& file_path) {
        auto dir_ = Replace(file_path, "\\", "/");
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(dir_, &dir, &file_name, '/');
        auto block = CreateVirtualBlock(root, dir);
        if(block == nullptr) {
            return nullptr;
        } else {
            auto found = block->node_map.find(file_name);
            if(found == block->node_map.end()) {
                auto node_name = block->path + file_name;
                auto node = std::make_shared<VirtualNode>();
                root->node_map[file_name] = node;
                node->name = node_name;
                node->vblock = block;
                block->node_map[file_name] = node;
                return node;
            } else {
                return found->second;
            }
        }
    }

    VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        // check invalid status
        if(dirs.size() == 0) return root;
        if(root == nullptr) return nullptr;
        if(level == dirs.size()) return root;
        // find if dir exist
        auto found = root->block_map.find(dirs[level]);
        // if not exist, return null
        if(found == root->block_map.end()) { return nullptr; }
        // check return or go deeper
        VirtualBlockPtr sub_block = found->second;
        return FindVirtualBlock(sub_block, dirs, level + 1);
    }

    VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs) {
        return FindVirtualBlock(root, dirs, 0);
    }

    VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::string& dir) {
        auto dir_ = Replace(dir, "\\", "/");
        std::vector<std::string> dir_stack;
        SplitSingleChar(dir_, &dir_stack, '/');
        auto block = FindVirtualBlock(root, dir_stack);
        return block;
    }

    VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        // check invalid status
        if(dirs.size() == 0) return root;
        if(root == nullptr) return nullptr;
        if(level == dirs.size()) return root;
        // find if dir exist
        auto found = root->block_map.find(dirs[level]);
        // we reach deepest dir
        if(found == root->block_map.end()) { return root; }
        // check return or go deeper
        VirtualBlockPtr sub_block = found->second;
        return FindVirtualBlock(sub_block, dirs, level + 1);
    }

    VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs) {
        return FindDeepestExistVirtualBlock(root, dirs, 0);
    }

    VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::string& dir) {
        auto dir_ = Replace(dir, "\\", "/");
        std::vector<std::string> dir_stack;
        SplitSingleChar(dir_, &dir_stack, '/');
        auto block = FindDeepestExistVirtualBlock(root, dir_stack);
        return block;
    }

    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, const std::string& name) {
        // find file parent block
        auto block = FindVirtualBlock(root, dirs, 0);
        if(block == nullptr) return nullptr;
        // find target file
        auto found = root->node_map.find(name);
        if(found == root->node_map.end()) { return nullptr; }
        return found->second;
    }

    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& dir, const std::string& name) {
        auto dir_ = Replace(dir, "\\", "/");
        std::vector<std::string> dir_stack;
        SplitSingleChar(dir_, &dir_stack, '/');
        auto node = FindVirtualNode(root, dir_stack, name);
        return node;
    }

    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& file_path) {
        auto dir_ = Replace(file_path, "\\", "/");
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(dir_, &dir, &file_name, '/');
        std::vector<std::string> dir_stack;
        SplitSingleChar(dir, &dir_stack, '/');
        auto node = FindVirtualNode(root, dir_stack, file_name);
        return node;
    }
}
