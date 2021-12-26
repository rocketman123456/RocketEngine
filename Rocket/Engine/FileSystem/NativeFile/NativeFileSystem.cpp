#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/NativeFile/NativeUtils.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

#include <exception>
#include <stdexcept>
#include <vector>
#include <fstream>

namespace Rocket {
    NativeFileSystem::NativeFileSystem(const std::string& real_path) 
        : FileSystem(real_path, "/") {}
    NativeFileSystem::NativeFileSystem(const std::string& real_path, const std::string& virtual_path)
        : FileSystem(real_path, virtual_path) {}

    void NativeFileSystem::Initialize() {
        if(IsInitialized()) {
            RK_INFO(File, "File System Already Initialized");
            return;
        }

        // Normalize Path
        NormalizePath();
        // Check File System
        CheckFileSystem();
        // Init Root Block
        root = std::make_shared<VirtualBlock>();
        root->path = virtual_path;
        // Get Root Name
        GetRootName();
        // Build up virtual blocks recurisively
        BuildVirtualSystem();
        // Finish
        is_initialized = true;
    }

    void NativeFileSystem::NormalizePath() {
        real_path = Replace(real_path, "\\", "/");
        if(!EndsWith(real_path, "/")) 
            real_path += "/";
        virtual_path = Replace(virtual_path, "\\", "/");
        if(!EndsWith(virtual_path, "/")) 
            virtual_path += "/";
        if(!StartsWith(virtual_path, "/"))
            virtual_path = "/" + virtual_path;
        RK_TRACE(File, "Current path: {}, {}", virtual_path, real_path);
    }

    void NativeFileSystem::CheckFileSystem() {
        std::filesystem::path basic = real_path;
        if(!std::filesystem::exists(basic)) {
            RK_TRACE(File, "Native File System {} Not Exist", real_path);
            throw std::invalid_argument("Native File System Not Exist");
        }
        if(!std::filesystem::is_directory(basic)) {
            RK_TRACE(File, "Native File System {} Not A Path", real_path);
            throw std::invalid_argument("Native File System Not A Path");
        }
    }

    void NativeFileSystem::GetRootName() {
        std::vector<std::string> dir_stack;
        SplitSingleChar(virtual_path, &dir_stack, '/');
        if(dir_stack.size() > 0) {
            root->name = dir_stack.at(dir_stack.size() - 1);
        } else {
            root->name = "/";
        }
    }

    void NativeFileSystem::BuildVirtualSystem() {
        std::filesystem::path basic = real_path;
        node_map.clear();
        block_map.clear();
        RK_INFO(File, "Build Up Virtual Blocks");
        BuildVirtualSystem(basic, root);
    }

    void NativeFileSystem::BuildVirtualSystem(const std::filesystem::path& path, VirtualBlockPtr& root) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            auto filename = entry.path().filename();
            auto filename_str = filename.u8string();
            if (std::filesystem::is_directory(entry.status())) {
                // Add Block
                auto block = std::make_shared<VirtualBlock>();
                root->block_map[filename_str] = block;
                block->parent = root;
                block->name = filename_str;
                block->path = root->path + filename_str + "/";
                // Create Block Index
                block_map[block->path] = block;
                // RK_TRACE(File, "Block Name: {}", filename_str);
                RK_TRACE(File, "Block Path: {}", block->path);
                BuildVirtualSystem(entry, block);
            } else if (std::filesystem::is_regular_file(entry.status())) {
                // Add Node
                auto node_name = root->path + filename_str;
                auto node = std::make_shared<VirtualNode>();
                root->node_map[filename_str] = node;
                node->path = root->path;
                node->name = filename_str;
                node->vblock = root;
                // Create Node Index
                node_map[node_name] = node;
                // RK_TRACE(File, "Node Name: {}", node->file_name);
                RK_TRACE(File, "Node Path: {}", node->path + node->name);
            } else {
                // Except Special File
                auto abs_path = std::filesystem::absolute(entry);
                RK_TRACE(File, "Not Regular File: {}", abs_path.u8string());
                continue;
            }
        }
    }

    void NativeFileSystem::Finalize() {
        root = nullptr;
        real_path = "";
        virtual_path = "";
        node_map.clear();
        block_map.clear();
        is_initialized = false;
    }

    void NativeFileSystem::SetVirtualPath(const std::string& vpath) {
        virtual_path = vpath;
        if(!IsInitialized()) { return; }

        // Normalize Path
        NormalizePath();
        // Init Root Block
        root = nullptr;
        root = std::make_shared<VirtualBlock>();
        root->path = virtual_path;
        // Get Root Name
        GetRootName();
        // Build up virtual blocks recurisively
        BuildVirtualSystem();
    }

    void NativeFileSystem::SetRealPath(const std::string& rpath) {
        real_path = rpath;
        if(!IsInitialized()) { return; }

        // Normalize Path
        NormalizePath();
        // Check File System
        CheckFileSystem();
        // Init Root Block
        root = nullptr;
        root = std::make_shared<VirtualBlock>();
        root->path = virtual_path;
        // Get Root Name
        GetRootName();
        // Build up virtual blocks recurisively
        BuildVirtualSystem();
    }

    bool NativeFileSystem::IsReadOnly() const {
        return IsNativeReadOnly(real_path);
    }

    FilePtr NativeFileSystem::GetFilePointer(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        auto temp = file_path.substr(virtual_path.size());
        auto full_path = real_path + temp;
        auto file = std::make_shared<NativeFile>(full_path, file_path);
        return file;
    }

    bool NativeFileSystem::CreateFile(const std::string& file_path) {
        if(IsFileExists(file_path)) {
            RK_WARN(File, "File Existed {}", file_path);
            return false;
        }
        // Create Dir in Real File System
        std::string temp_path = Replace(file_path, "\\", "/");
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(temp_path, &dir, &file_name, '/');
        CreateDir(dir);
        // Create File in Real File System
        auto temp = file_path.substr(virtual_path.size());
        auto full_path = real_path + temp;
        std::fstream stream(full_path, std::fstream::out);
        if(!stream.is_open()) {
            RK_ERROR(File, "Failed to create file: {}, {}", file_path, full_path);
            return false;
        }
        // Add info to VFS
        

        return true;
    }

    bool NativeFileSystem::RemoveFile(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Existed {}", file_path);
            return false;
        }
        // Remove File in Real File System
        // Remove info in VFS
        return true;
    }

    bool NativeFileSystem::CreateDir(const std::string& dir_path) {
        RK_WARN(File, "Create Dir Not Supported");
        return false;
    }

    bool NativeFileSystem::RemoveDir(const std::string& dir_path) {
        RK_WARN(File, "Remove Dir Not Supported");
        return false;
    }
}
