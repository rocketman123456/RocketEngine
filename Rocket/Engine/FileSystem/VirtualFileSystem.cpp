#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "Utils/StringUtils.h"

#include <vector>
#include <string>

namespace Rocket {
    VirtualBlockPtr VirtualFileSystem::CreateVirtualBlock(VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        if(root == nullptr) return nullptr;
        if(dirs.size() == 0 || level == dirs.size()) return root;
        auto found = root->block_map.find(dirs[level]);
        if(found == root->block_map.end()) {
            VirtualBlockPtr block = std::make_shared<VirtualBlock>();
            block->parent = root;
            block->name = dirs[level];
            block->path = root->path + block->name + "/";
            root->block_map[block->name] = block;
            block_map[block->path] = block;
            RK_TRACE(File, "Block Path: {}", block->path);
            return CreateVirtualBlock(block, dirs, level + 1);
        } else {
            return CreateVirtualBlock(found->second, dirs, level + 1);
        }
    }

    void VirtualFileSystem::SetupBlockFileSystem(VirtualBlockPtr& root, const FileSystemPtr& fs) {
        root->file_system = fs;
        for(auto& block : root->block_map) {
            SetupBlockFileSystem(block.second, fs);
        }
    }

    bool VirtualFileSystem::MountFileSystem(const FileSystemPtr& fs, const std::string& path) {
        if(root == nullptr) {
            root = std::make_shared<VirtualBlock>();
            root->path = "/";
        }
        std::vector<std::string> dir_stack = {};
        SplitSingleChar(path, &dir_stack, '/');
        auto final_block = CreateVirtualBlock(root, dir_stack, 0);
        auto fs_root = fs->RootBlock();
        if(fs_root == nullptr) {
            RK_WARN(File, "Empty Root Block")
            return false;
        }
        if(final_block->parent != nullptr) {
            final_block->parent->block_map[fs_root->name] = fs_root;
        } else {
            root = fs_root;
        }

        SetupBlockFileSystem(fs_root, fs);

        // TODO : add index

        return true;
    }

    bool VirtualFileSystem::UnmountFileSystem(const FileSystemPtr& fs) {
        return UnmountFileSystem(fs->VirtualPath());
    }

    bool VirtualFileSystem::UnmountFileSystem(const std::string& path) {
        std::vector<std::string> dir_stack = {};
        SplitSingleChar(path, &dir_stack, '/');
        auto block = FindVirtualBlock(root, dir_stack);

        // TODO : remove index

        if(block == nullptr) {
            return false;
        } else {
            if(block->parent == nullptr) {
                root = std::make_shared<VirtualBlock>();
                root->path = "/";
            } else {
                auto found = block->parent->block_map.find(block->name);
                block->parent->block_map.erase(found);
            }
            return true;
        }
    }

    VNodeList VirtualFileSystem::VNodes(const std::string& dir) const {
        auto dir_ = Replace(dir, "\\", "/");
        std::vector<std::string> dir_stack;
        SplitSingleChar(dir_, &dir_stack, '/');
        auto block = FindVirtualBlock(root, dir_stack, 0);
        if(block == nullptr) return {};
        VNodeList nodes = {};
        for(auto item : block->node_map) {
            nodes.push_back(item.second);
        }
        return nodes;
    }

    VBlockList VirtualFileSystem::VBlocks(const std::string& dir) const {
        auto dir_ = Replace(dir, "\\", "/");
        std::vector<std::string> dir_stack;
        SplitSingleChar(dir_, &dir_stack, '/');
        auto block = FindVirtualBlock(root, dir_stack, 0);
        if(block == nullptr) return {};
        VBlockList blocks = {};
        for(auto item : block->block_map) {
            blocks.push_back(item.second);
        }
        return blocks;
    }
}
