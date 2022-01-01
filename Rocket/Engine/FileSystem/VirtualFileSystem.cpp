#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "Utils/StringUtils.h"
#include "Log/Instrumentor.h"

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
        } else {
            root = fs_root;
        }
        return true;
    }

    bool VirtualFileSystem::UnmountFileSystem(const FileSystemPtr& fs) {
        return UnmountFileSystem(fs->VirtualPath());
    }

    bool VirtualFileSystem::UnmountFileSystem(const std::string& path) {
        // 1. find target block
        auto block = FindVirtualBlock(root, path);
        if(block == nullptr) { return false; }
        // 2. finalize fs
        auto fs = block->file_system;
        fs->Finalize();
        // 3. correct other block's data 
        if(block->parent == nullptr) {
            root = std::make_shared<VirtualBlock>();
            root->path = "/";
        } else {
            auto found = block->parent->block_map.find(block->name);
            block->parent->block_map.erase(found);
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

    FilePtr VirtualFileSystem::GetFilePointer(const std::string& file_path) {
        RK_PROFILE_FUNCTION();
        auto node = FindVirtualNode(root, file_path);
        if(node == nullptr) {
            return nullptr;
        } else {
            return node->vblock->file_system->GetFilePointer(file_path);
        }
    }

    void VirtualFileSystem::OpenFile(const FilePtr& file, int32_t mode) {
        if(file == nullptr) return;
        file->Open(mode); 
    }

    void VirtualFileSystem::CloseFile(const FilePtr& file) { 
        if(file == nullptr) return;
        file->Close(); 
    }

    std::size_t VirtualFileSystem::ReadFile(const FilePtr& file, FileBuffer* data) { 
        if(file == nullptr) return std::size_t(0);
        return file->Read(data); 
    }

    std::size_t VirtualFileSystem::WriteFile(FilePtr& file, const FileBuffer& data) { 
        if(file == nullptr) return std::size_t(0);
        return file->Write(data); 
    }

    bool VirtualFileSystem::CreateFile(const std::string& file_path) {
        auto block = FindDeepestExistVirtualBlock(root, file_path);
        if(block == nullptr) {
            return false;
        } else {
            auto result = block->file_system->CreateFile(file_path);
            SetupBlockFileSystem(block->file_system->RootBlock(), block->file_system);
            return result;
        }
    }

    bool VirtualFileSystem::RemoveFile(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return false;
        } else {
            auto node = FindVirtualNode(root, file_path);
            return node->vblock->file_system->RemoveFile(file_path);
        }
    }

    bool VirtualFileSystem::CreateDir(const std::string& dir_path) {
        auto block = FindDeepestExistVirtualBlock(root, dir_path);
        if(block == nullptr) {
            return false;
        } else {
            auto result = block->file_system->CreateDir(dir_path);
            SetupBlockFileSystem(block->file_system->RootBlock(), block->file_system);
            return result;
        }
    }

    bool VirtualFileSystem::RemoveDir(const std::string& dir_path) {
        if(!IsDirExists(dir_path)) {
            RK_WARN(File, "Dir Not Exist {}", dir_path);
            return false;
        } else {
            auto block = FindVirtualBlock(root, dir_path);
            return block->file_system->RemoveDir(dir_path);
        }
    }
}
