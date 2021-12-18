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

    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, const std::string& name) {
        // find file parent block
        auto block = FindVirtualBlock(root, dirs, 0);
        if(block == nullptr) return nullptr;
        // find target file
        auto found = root->node_map.find(name);
        if(found == root->node_map.end()) { return nullptr; }
        return found->second;
    }
}
