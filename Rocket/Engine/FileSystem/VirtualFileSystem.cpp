#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "Utils/StringUtils.h"

#include <vector>
#include <string>

namespace Rocket {
    void VirtualFileSystem::SetupBlockFileSystem(VirtualBlockPtr& root, const FileSystemPtr& fs) {
        root->file_system = fs;
        for(auto& block : root->block_map) {
            SetupBlockFileSystem(block.second, fs);
        }
    }

    bool VirtualFileSystem::MountFileSystem(const FileSystemPtr& fs, const std::string& path) {
        // 1. Generate Root Block
        if(root == nullptr) {
            root = std::make_shared<VirtualBlock>();
            root->path = "/";
            //block_map[root->path] = root;
        }

        // 2. Initialize fs
        if(!fs->IsInitialized()) {
            fs->Initialize();
        }
        auto fs_root = fs->RootBlock();
        if(fs_root == nullptr) {
            RK_WARN(File, "Empty Root Block")
            return false;
        }

        // 3. generate target block
        auto final_block = CreateVirtualBlock(root, path);
        if(final_block == nullptr) {
            RK_WARN(File, "Failed to mount file system");
            return false;
        }

        // 4. Setup fs for blocks
        SetupBlockFileSystem(fs_root, fs);

        // 5. insert block data
        if(final_block->parent != nullptr) {
            fs_root->parent = final_block->parent;
            final_block->parent->block_map[fs_root->name] = fs_root;
            //block_map[fs_root->path] = fs_root;
        } else {
            root = fs_root;
            //block_map[fs_root->path] = fs_root;
        }

        // 6. update cached index
        auto block_map_ = fs->BlockMap();
        auto node_map_ = fs->NodeMap();
        //node_map.insert(node_map_.begin(), node_map_.end());
        //block_map.insert(block_map_.begin(), block_map_.end());
        return true;
    }

    bool VirtualFileSystem::UnmountFileSystem(const FileSystemPtr& fs) {
        return UnmountFileSystem(fs->VirtualPath());
    }

    bool VirtualFileSystem::UnmountFileSystem(const std::string& path) {
        // 1. find target block
        auto block = FindVirtualBlock(root, path);
        if(block == nullptr) { return false; }
        // 2. remove cached index
        // RemoveCacheInfo(block);
        // 3. finalize fs
        auto fs = block->file_system;
        fs->Finalize();
        // 4. correct other block's data 
        if(block->parent == nullptr) {
            root = std::make_shared<VirtualBlock>();
            root->path = "/";
            //block_map[root->path] = root;
        } else {
            auto found = block->parent->block_map.find(block->name);
            block->parent->block_map.erase(found);
            // auto found_2 = block_map.find(block->path);
            //block_map.erase(found_2);
        }
        return true;
    }

    VNodeList VirtualFileSystem::VNodes(const std::string& dir) const {
        auto block = FindVirtualBlock(root, dir);
        if(block == nullptr) return {};
        VNodeList nodes = {};
        for(auto item : block->node_map) {
            nodes.push_back(item.second);
        }
        return nodes;
    }

    VBlockList VirtualFileSystem::VBlocks(const std::string& dir) const {
        auto block = FindVirtualBlock(root, dir);
        if(block == nullptr) return {};
        VBlockList blocks = {};
        for(auto item : block->block_map) {
            blocks.push_back(item.second);
        }
        return blocks;
    }

    bool VirtualFileSystem::IsFileExists(const std::string& file_path) const {
        auto node = FindVirtualNode(root, file_path);
        if(node == nullptr) {
            return false;
        } else {
            return true;
        }
    }

    bool VirtualFileSystem::IsDirExists(const std::string& dir_path) const {
        auto block = FindVirtualBlock(root, dir_path);
        if(block == nullptr) {
            return false;
        } else {
            return true;
        }
    }

    bool VirtualFileSystem::IsFile(const std::string& file_path) const {
        return IsFileExists(file_path); 
    }

    bool VirtualFileSystem::IsDir(const std::string& dir_path) const { 
        return IsDirExists(dir_path); 
    }

    bool VirtualFileSystem::IsFileReadOnly(const std::string& file_path) const {
        // TODO : 
        return false;
    }

    bool VirtualFileSystem::IsDirReadOnly(const std::string& file_path) const {
        // TODO :
        return false;
    }

    FilePtr VirtualFileSystem::GetFilePointer(const std::string& file_path) {
        auto node = FindVirtualNode(root, file_path);
        if(node == nullptr) {
            return nullptr;
        } else {
            return node->vblock->file_system->GetFilePointer(file_path);
        }
        // auto found = node_map.find(file_path);
        // if(found == node_map.end()) {
        //     return nullptr;
        // } else {
        //     return found->second->vblock->file_system->GetFilePointer(file_path);
        // }
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


    bool VirtualFileSystem::CreateFile(const std::string& file_path) {
        return false;
    }

    bool VirtualFileSystem::RemoveFile(const std::string& file_path) {
        return false;
    }

    bool VirtualFileSystem::CreateDir(const std::string& dir_path) {
        return false;
    }

    bool VirtualFileSystem::RemoveDir(const std::string& dir_path) {
        return false;
    }
}
