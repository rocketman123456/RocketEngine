#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"

namespace Rocket {
    MemoryFileSystem::MemoryFileSystem(const std::string& real_path) 
        : FileSystem(real_path, "/") {}
    MemoryFileSystem::MemoryFileSystem(const std::string& real_path, const std::string& virtual_path)
        : FileSystem(real_path, virtual_path) {}

    void MemoryFileSystem::Initialize() {
        if(IsInitialized()) {
            RK_INFO(File, "File System Already Initialized");
            return;
        }
        root = std::make_shared<VirtualBlock>();
        root->path = virtual_path;
        is_initialized = true;
    }

    void MemoryFileSystem::Finalize() {
        root = nullptr;
        real_path = "";
        virtual_path = "";
        is_initialized = false;
        for(auto file : file_map) {
            if(file.second->data() != nullptr) {
                delete [] file.second->data();
            }
        }
        file_map.clear();
    }

    void MemoryFileSystem::SetVirtualPath(const std::string& vpath) {
        virtual_path = vpath;
    }

    void MemoryFileSystem::SetRealPath(const std::string& rpath) {
        real_path = rpath;
    }

    FilePtr MemoryFileSystem::GetFilePointer(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        std::string temp_path = Replace(file_path, "\\", "/");
        auto full_path = temp_path.substr(virtual_path.size());
        auto file = std::make_shared<MemoryFile>(full_path, temp_path);
        // Set File Buffer Data
        auto found = file_map.find(temp_path);
        if(found == file_map.end()) {
            RK_WARN(File, "Empty File Data: {}", temp_path);
            file->SetFileBuffer(nullptr);
        } else {
            file->SetFileBuffer(found->second);
        }
        return file;
    }

    bool MemoryFileSystem::CreateFile(const std::string& file_path) {
        if(IsFileExists(file_path)) {
            RK_WARN(File, "File Existed {}", file_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(file_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Create Virtual Block
        auto final_block = CreateVirtualBlock(root, dir);
        // Create Memory File
        auto file = std::make_shared<FileBuffer>(nullptr, std::size_t(0));
        file_map[file_path] = file;
        // Add info to VFS
        auto node = std::make_shared<VirtualNode>();
        node->name = file_name;
        node->path = final_block->path;
        node->vblock = final_block;
        final_block->node_map[file_name] = node;
        return true;
    }

    bool MemoryFileSystem::RemoveFile(const std::string& file_path) {
        if(IsFileExists(file_path)) {
            RK_WARN(File, "File Not Existed {}", file_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(file_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Remove file in vfs
        VirtualBlockPtr block = FindVirtualBlock(root, dir);
        if(block == nullptr) {
            RK_WARN(File, "Virtual Block Not Existed {}, MUST Have Something Wrong", file_path);
            return false;
        }
        auto found = block->node_map.find(file_name);
        if(found == block->node_map.end()) {
            RK_WARN(File, "Virtual Node Not Existed {}, MUST Have Something Wrong", file_path);
            return false;
        } else {
            block->node_map.erase(found);
            return true;
        }
        // remove memory file storage
        auto found_file = file_map.find(file_path);
        if(found_file->second->data() != nullptr) {
            delete [] found_file->second->data();
        }
        file_map.erase(found_file);
        return true;
    }

    bool MemoryFileSystem::CreateDir(const std::string& dir_path) {
        if(IsFileExists(dir_path)) {
            RK_WARN(File, "File Existed {}", dir_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(dir_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        // Create Virtual Block
        auto final_block = CreateVirtualBlock(root, sub_path);
        return true;
    }

    void MemoryFileSystem::RemoveFileRecursive(VirtualBlockPtr& block) {
        for(auto file : block->node_map) {
            auto found = file_map.find(file.second->path + file.second->name);
            auto file_ptr = found->second;
            if(file_ptr->data() != nullptr) {
                delete [] file_ptr->data();
            }
            file_map.erase(found);
        }
        for(auto block : block->block_map) {
            RemoveFileRecursive(block.second);
        }
    }

    bool MemoryFileSystem::RemoveDir(const std::string& dir_path) {
        if(!IsDirExists(dir_path)) {
            RK_WARN(File, "Dir Not Existed {}", dir_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(dir_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        // Remove File Storage
        VirtualBlockPtr block = FindVirtualBlock(root, sub_path);
        RemoveFileRecursive(block);
        // Remove Info
        if(block->parent != nullptr) {
            auto found = block->parent->block_map.find(block->name);
            block->parent->block_map.erase(found);
        } else {
            block->block_map.clear();
            block->node_map.clear();
            block->file_system.reset();
        }
        return true;
    }
}
