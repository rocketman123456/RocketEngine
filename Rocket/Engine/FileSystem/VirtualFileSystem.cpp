#include "FileSystem/VirtualFileSystem.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

#include <exception>
#include <stdexcept>

namespace Rocket {
    void VirtualFileSystem::MountFileSystem(const std::string& alias, FileSystemPtr filesystem) {
        std::vector<std::string> path_stack;
        SplitSingleChar(alias, path_stack, '/');
        // Build up vfs blocks
        auto result = FindVirtualBlock(root_block, path_stack);
        if(result == nullptr) {
            // Build up
        }
        // Build up file system blocks
        filesystem->SetAliasPath(alias);
        filesystem->Initialize();
        auto block = filesystem->RootBlock();
        // Insert file system root block
        result = FindVirtualBlock(root_block, path_stack);
        if(result == nullptr) {
            throw std::runtime_error("Cannot find mount point");
        }
        
    }

    void VirtualFileSystem::UnmountFileSystem(const std::string& alias) {
        std::vector<std::string> path_stack;
        SplitSingleChar(alias, path_stack, '/');
        auto result = FindVirtualBlock(root_block, path_stack);
        if(result != nullptr) {
            // Erase sub dirs
            result->block_map.clear();
            result->node_map.clear();
            // Check if file system still exist
            if(result->parent != nullptr) {
                if(result->parent->file_system != result->file_system) {
                    // if all block using this file system is unmounted
                    // finalize this file system and remove it
                    result->file_system->Finalize();
                    result->file_system = result->parent->file_system;
                }
            } else {
                // if this is the root file system,
                RK_WARN(File, "Unmount Root File System");
                result->file_system->Finalize();
                result->file_system = nullptr;
                root_block = nullptr;
            }
        }
    }

    void VirtualFileSystem::SetupFileSystemRecurisively(VirtualBlockPtr block, const FileSystemPtr& file_system) {
        if(block == nullptr) {
            return;
        }
        block->file_system = file_system;
        for(auto child : block->block_map) {
            auto sub_block = child.second;
            if(sub_block != nullptr) {
                SetupFileSystemRecurisively(sub_block, file_system);
            }
        }
    }

    FilePtr VirtualFileSystem::OpenFile(const std::string& filePath, FileMode::FileOperate mode) {
        return nullptr;
    }

    void VirtualFileSystem::CloseFile(FilePtr file) {
    }
}
