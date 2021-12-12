#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "Log/Log.h"

#include <exception>
#include <stdexcept>
#include <vector>

namespace Rocket {
    NativeFileSystem::NativeFileSystem(const std::string& basic_) 
        : basic_path(basic_), alias_path("/") {}
    NativeFileSystem::NativeFileSystem(const std::string& basic_, const std::string& alias_)
        : basic_path(basic_), alias_path(alias_) {}

    void NativeFileSystem::Initialize() {
        if(is_initialized) return;
        RK_TRACE(File, "Current path: {}, {}", basic_path, alias_path);
        std::filesystem::path basic = basic_path;
        // Check File Path
        if(!std::filesystem::exists(basic)) {
            RK_TRACE(File, "Native File System File Path {} Not Exist", basic_path);
            throw std::invalid_argument("Native File System File Path Not Exist");
        }
        if(!std::filesystem::is_directory(basic)) {
            RK_TRACE(File, "Native File System File Path {} Not A Path", basic_path);
            throw std::invalid_argument("Native File System File Path Not A Path");
        }
        // Init Root Block
        root = std::make_shared<VirtualBlock>();
        root->block_path = alias_path;
        // Get Root Name
        std::vector<std::string> dir_stack;
        Replace(alias_path, "\\", "/");
        SplitSingleChar(alias_path, dir_stack, '/');
        if(dir_stack.size() > 0)
            root->block_name = dir_stack.at(dir_stack.size() - 1);
        else 
            root->block_name = "/";

        // Build up virtual blocks recurisively
        RK_INFO(File, "Build Up Virtual Blocks");
        BuildUpVirtualBlocks(basic, root);

        is_initialized = true;
    }

    void NativeFileSystem::BuildUpVirtualBlocks(const std::filesystem::path& path, VirtualBlockPtr& root) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            auto filename = entry.path().filename();
            auto filename_str = filename.u8string();
            if (std::filesystem::is_directory(entry.status())) {
                // Add Block
                auto block = std::make_shared<VirtualBlock>();
                root->block_map[filename_str] = block;
                block->parent = root;
                block->block_name = filename_str;
                block->block_path = root->block_path + filename_str + "/";
                //RK_TRACE(File, "Block Name: {}", filename_str);
                RK_TRACE(File, "Block Path: {}", block->block_path);
                BuildUpVirtualBlocks(entry, block);
            } else if (std::filesystem::is_regular_file(entry.status())) {
                // Add Node
                auto node_name = root->block_path + filename_str;
                auto node = std::make_shared<VirtualNode>(node_name);
                root->node_map[filename_str] = node;
                node->virtual_block = root;
                //RK_TRACE(File, "Node Name: {}", node->file_name);
                RK_TRACE(File, "Node Path: {}", node->file_path + node->file_name);
            } else {
                auto abs_path = std::filesystem::absolute(entry);
                RK_TRACE(File, "Not Regular File: {}", abs_path.u8string());
            }
        }
    }

    void NativeFileSystem::Finalize() {
        root = nullptr;
        basic_path = "";
        alias_path = "";
        is_initialized = false;
    }

    const FileList& NativeFileSystem::Files(const std::string& dir) const {
        std::vector<std::string> dir_stack;
        Replace(dir, "\\", "/");
        SplitSingleChar(dir, dir_stack, '/');
    }

    FilePtr NativeFileSystem::OpenFile(const std::string& file_path, int32_t mode) {

    }

    void NativeFileSystem::CloseFile(const FilePtr& file) {

    }

    bool NativeFileSystem::CreateFile(const std::string& file_path) {

    }

    bool NativeFileSystem::RemoveFile(const std::string& file_path) {

    }

    bool NativeFileSystem::CreateDir(const std::string& dir_path) {

    }

    bool NativeFileSystem::RemoveDir(const std::string& dir_path) {

    }

    bool NativeFileSystem::IsFileExists(const std::string& file_path) const {

    }

    bool NativeFileSystem::IsFile(const std::string& file_path) const {

    }

    bool NativeFileSystem::IsDir(const std::string& file_path) const {

    }

    bool NativeFileSystem::IsReadOnly() const {

    }
}
