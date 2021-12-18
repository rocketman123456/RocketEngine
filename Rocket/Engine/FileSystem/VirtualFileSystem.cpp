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

        if(!fs->IsInitialized()) {
            fs->Initialize();
        }
        auto fs_root = fs->RootBlock();
        if(fs_root == nullptr) {
            RK_WARN(File, "Empty Root Block")
            return false;
        }

        std::vector<std::string> dir_stack = {};
        SplitSingleChar(path, &dir_stack, '/');
        auto final_block = CreateVirtualBlock(root, dir_stack, 0);

        if(final_block == nullptr) {
            return false;
        }

        SetupBlockFileSystem(fs_root, fs);

        if(final_block->parent != nullptr) {
            fs_root->parent = final_block->parent;
            final_block->parent->block_map[fs_root->name] = fs_root;
            block_map[fs_root->path] = fs_root;
        } else {
            root = fs_root;
            block_map[fs_root->path] = fs_root;
        }

        auto block_map_ = fs->BlockMap();
        auto node_map_ = fs->NodeMap();

        node_map.insert(node_map_.begin(), node_map_.end());
        block_map.insert(block_map_.begin(), block_map_.end());

        return true;
    }

    void VirtualFileSystem::RemoveVirtualBlock(VirtualBlockPtr& root) {
        for(auto& node : root->node_map) {
            auto found = node_map.find(node.second->path + node.second->name);
            if(found != node_map.end()) {
                node_map.erase(found);
            }
        }
        for(auto& block : root->block_map) {
            auto found = block_map.find(block.second->path);
            if(found != block_map.end()) {
                block_map.erase(found);
            }
            RemoveVirtualBlock(block.second);
        }
    }

    bool VirtualFileSystem::UnmountFileSystem(const FileSystemPtr& fs) {
        return UnmountFileSystem(fs->VirtualPath());
    }

    bool VirtualFileSystem::UnmountFileSystem(const std::string& path) {
        std::vector<std::string> dir_stack = {};
        SplitSingleChar(path, &dir_stack, '/');
        auto block = FindVirtualBlock(root, dir_stack);

        if(block == nullptr) {
            return false;
        }

        auto fs = block->file_system;

        RemoveVirtualBlock(block);

        fs->Finalize();

        if(block == nullptr) {
            return false;
        } else {
            if(block->parent == nullptr) {
                root = std::make_shared<VirtualBlock>();
                root->path = "/";
            } else {
                auto found = block->parent->block_map.find(block->name);
                block->parent->block_map.erase(found);
                auto found_2 = block_map.find(block->path);
                block_map.erase(found_2);
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

    FilePtr VirtualFileSystem::GetFilePointer(const std::string& file_path) {
        auto found = node_map.find(file_path);
        if(found == node_map.end()) {
            return nullptr;
        } else {
            return found->second->vblock->file_system->GetFilePointer(file_path);
        }
    }

    void VirtualFileSystem::OpenFile(const FilePtr& file, int32_t mode) {
        file->Open(mode);
    }

    void VirtualFileSystem::CloseFile(const FilePtr& file) {
        file->Close();
    }

    std::size_t VirtualFileSystem::ReadFile(const FilePtr& file, FileBuffer* data) {
        return file->Read(data);
    }

    std::size_t VirtualFileSystem::WriteFile(FilePtr& file, const FileBuffer& data) {
        return file->Write(data);
    }
}
