#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "FileSystem/NativeFile/NativeUtils.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

#include <exception>
#include <stdexcept>
#include <vector>

namespace Rocket {
    NativeFileSystem::NativeFileSystem(const std::string& real_path) 
        : real_path(real_path), virtual_path("/") {}
    NativeFileSystem::NativeFileSystem(const std::string& real_path, const std::string& virtual_path)
        : real_path(real_path), virtual_path(virtual_path) {}

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

    void NativeFileSystem::Finalize() {
        root = nullptr;
        real_path = "";
        virtual_path = "";
        node_map.clear();
        block_map.clear();
        is_initialized = false;
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
                RK_TRACE(File, "Block Name: {}", filename_str);
                // RK_TRACE(File, "Block Path: {}", block->path);
                BuildVirtualSystem(entry, block);
            } else if (std::filesystem::is_regular_file(entry.status())) {
                // Add Node
                auto node_name = root->path + filename_str;
                auto node = std::make_shared<VirtualNode>();
                root->node_map[filename_str] = node;
                node->name = node_name;
                node->vblock = root;
                // Create Node Index
                node_map[node->path + node->name] = node;
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

    VNodeList NativeFileSystem::VNodes(const std::string& dir) const {
        VNodeList nodes = {};
        for(auto& node : node_map) {
            nodes.push_back(node.second);
        }
        return nodes;
    }

    VBlockList NativeFileSystem::VBlocks(const std::string& dir) const {
        VBlockList blocks = {};
        for(auto& block : block_map) {
            blocks.push_back(block.second);
        }
        return blocks;
    }

    bool NativeFileSystem::IsFileExists(const std::string& file_path) const {
        auto found = node_map.find(file_path);
        if(found == node_map.end())
            return false;
        else
            return true;
    }

    bool NativeFileSystem::IsDirExists(const std::string& dir_path) const {
        auto found = block_map.find(dir_path);
        if(found == block_map.end())
            return false;
        else
            return true;
    }

    bool NativeFileSystem::IsFile(const std::string& file_path) const {
        return IsFileExists(file_path);
    }

    bool NativeFileSystem::IsDir(const std::string& dir_path) const {
        return IsDirExists(dir_path);
    }

    bool NativeFileSystem::IsReadOnly() const {
        return IsNativeReadOnly(real_path);
    }

    FilePtr NativeFileSystem::OpenFile(const std::string& file_path, int32_t mode) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        auto temp = file_path.substr(virtual_path.size());
        auto full_path = real_path + temp;
        auto file = std::make_shared<NativeFile>(full_path, file_path);
        file->Open(mode);
        return file;
    }

    void NativeFileSystem::CloseFile(const FilePtr& file) {
        file->Close();
    }

    std::size_t NativeFileSystem::ReadFile(const FilePtr& file, FileBuffer* data) {
        return file->Read(data);
    }

    std::size_t NativeFileSystem::WriteFile(FilePtr& file, const FileBuffer& data) {
        return file->Write(data);
    }

    bool NativeFileSystem::CreateFile(const std::string& file_path) {
        RK_WARN(File, "Create File Not Supported");
        return false;
    }

    bool NativeFileSystem::RemoveFile(const std::string& file_path) {
        RK_WARN(File, "Remove File Not Supported");
        return false;
    }

    std::size_t NativeFileSystem::FileSize(const FilePtr& file) const {
        return file->Size();
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
