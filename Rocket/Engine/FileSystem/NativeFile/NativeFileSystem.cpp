#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/NativeFile/NativeUtils.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "Utils/StringUtils.h"
#include "Log/Instrumentor.h"
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
        RK_PROFILE_FUNCTION();
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
        RK_PROFILE_FUNCTION();
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
        RK_PROFILE_FUNCTION();
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
        RK_PROFILE_FUNCTION();
        std::vector<std::string> dir_stack;
        SplitSingleChar(virtual_path, &dir_stack, '/');
        if(dir_stack.size() > 0) {
            root->name = dir_stack.at(dir_stack.size() - 1);
        } else {
            root->name = "/";
        }
    }

    void NativeFileSystem::BuildVirtualSystem() {
        RK_PROFILE_FUNCTION();
        std::filesystem::path basic = real_path;
        RK_INFO(File, "Build Up Virtual Blocks");
        BuildVirtualSystem(basic, root);
    }

    void NativeFileSystem::BuildVirtualSystem(const std::filesystem::path& path, VirtualBlockPtr& root) {
        RK_PROFILE_FUNCTION();
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
        is_initialized = false;
    }

    void NativeFileSystem::SetVirtualPath(const std::string& vpath) {
        RK_PROFILE_FUNCTION();
        virtual_path = vpath;
        if(!IsInitialized()) { return; }
        // Normalize Path
        NormalizePath();
        // Init Root Block
        root->path = virtual_path;
        root->node_map.clear();
        root->block_map.clear();
        // Get Root Name
        GetRootName();
        // Build up virtual blocks recurisively
        BuildVirtualSystem();
    }

    void NativeFileSystem::SetRealPath(const std::string& rpath) {
        RK_PROFILE_FUNCTION();
        real_path = rpath;
        if(!IsInitialized()) { return; }
        // Normalize Path
        NormalizePath();
        // Check File System
        CheckFileSystem();
        // Init Root Block
        root->path = virtual_path;
        root->node_map.clear();
        root->block_map.clear();
        // Get Root Name
        GetRootName();
        // Build up virtual blocks recurisively
        BuildVirtualSystem();
    }

    bool NativeFileSystem::IsReadOnly() const {
        return IsNativeReadOnly(real_path);
    }

    FilePtr NativeFileSystem::GetFilePointer(const std::string& file_path) {
        RK_PROFILE_FUNCTION();
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
        // Get File Dir and Name
        std::string temp_path = Replace(file_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Create Dir in Real File System
        auto full_path = real_path + dir;
        std::filesystem::create_directories(full_path);
        // Add info to VFS
        auto final_block = CreateVirtualBlock(root, dir);
        // Create File in Real File System
        std::fstream stream(full_path, std::fstream::out);
        if(!stream.is_open()) {
            RK_ERROR(File, "Failed to create file: {}, {}", file_path, full_path);
            return false;
        }
        // Add info to VFS
        auto node = std::make_shared<VirtualNode>();
        node->name = file_name;
        node->path = final_block->path;
        node->vblock = final_block;
        final_block->node_map[file_name] = node;
        return true;
    }

    bool NativeFileSystem::RemoveFile(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Existed {}", file_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(file_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Remove File in Real File System
        std::string real_file_path = real_path + sub_path;
        std::filesystem::remove(real_file_path);
        // Remove File Info in VFS
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
    }

    bool NativeFileSystem::CreateDir(const std::string& dir_path) {
        if(IsDirExists(dir_path)) {
            RK_WARN(File, "Dir Existed {}", dir_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(dir_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        std::string dir;
        std::string file_name;
        SplitLastSingleChar(sub_path, &dir, &file_name, '/');
        // Create Dir in Real File System
        auto full_path = real_path + dir;
        std::filesystem::create_directories(full_path);
        // Add info to VFS
        auto final_block = CreateVirtualBlock(root, dir);
        return true;
    }

    bool NativeFileSystem::RemoveDir(const std::string& dir_path) {
        if(!IsDirExists(dir_path)) {
            RK_WARN(File, "Dir Not Existed {}", dir_path);
            return false;
        }
        // Get File Dir and Name
        std::string temp_path = Replace(dir_path, "\\", "/");
        std::string sub_path = temp_path.substr(virtual_path.size());
        // Remove Dir in Real File System
        auto full_path = real_path + sub_path;
        std::filesystem::remove_all(full_path);
        // Remove Info
        VirtualBlockPtr block = FindVirtualBlock(root, sub_path);
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
