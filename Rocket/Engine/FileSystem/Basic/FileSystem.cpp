#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "Utils/StringUtils.h"
#include "Log/Instrumentor.h"

namespace Rocket {
    FileSystem::FileSystem(const std::string& real_path, const std::string& virtual_path)
        : real_path(real_path), virtual_path(virtual_path) {}

    VNodeList FileSystem::VNodes(const std::string& dir) const {
        auto block = FindVirtualBlock(root, dir);
        if(block == nullptr) return {};
        VNodeList nodes = {};
        for(auto item : block->node_map) {
            nodes.push_back(item.second);
        }
        return nodes;
    }

    VBlockList FileSystem::VBlocks(const std::string& dir) const {
        auto block = FindVirtualBlock(root, dir);
        if(block == nullptr) return {};
        VBlockList blocks = {};
        for(auto item : block->block_map) {
            blocks.push_back(item.second);
        }
        return blocks;
    }

    bool FileSystem::IsFileExists(const std::string& file_path) const {
        RK_PROFILE_FUNCTION();
        std::string temp_path = file_path.substr(virtual_path.size());
        auto node = FindVirtualNode(root, temp_path);
        if(node == nullptr)
            return false;
        else
            return true;
    }

    bool FileSystem::IsDirExists(const std::string& dir_path) const {
        RK_PROFILE_FUNCTION();
        std::string temp_path = dir_path.substr(virtual_path.size());
        auto block = FindVirtualBlock(root, temp_path);
        if(block == nullptr)
            return false;
        else
            return true;
    }

    bool FileSystem::IsFile(const std::string& file_path) const {
        return IsFileExists(file_path); 
    }

    bool FileSystem::IsDir(const std::string& dir_path) const { 
        return IsDirExists(dir_path); 
    }

    void FileSystem::OpenFile(const FilePtr& file, int32_t mode) { 
        if(file == nullptr) return;
        file->Open(mode); 
    }

    void FileSystem::CloseFile(const FilePtr& file) { 
        if(file == nullptr) return;
        file->Close(); 
    }

    std::size_t FileSystem::ReadFile(const FilePtr& file, FileBuffer* data) { 
        if(file == nullptr) return std::size_t(0);
        return file->Read(data); 
    }

    std::size_t FileSystem::WriteFile(FilePtr& file, const FileBuffer& data) { 
        if(file == nullptr) return std::size_t(0);
        return file->Write(data); 
    }

    bool FileSystem::MoveFile(const std::string& src, const std::string& dst) {
        RK_WARN(File, "Move File Not Supported");
        return false;
    }

    bool FileSystem::RenameFile(const std::string& src, const std::string& dst) {
        RK_WARN(File, "Rename File Not Supported");
        return false;
    }

    bool FileSystem::CopyFile(const std::string& src, const std::string& dst) {
        RK_WARN(File, "Copy File Not Supported");
        return false;
    }

    std::size_t FileSystem::FileSize(const FilePtr& file) const {
        return file->Size();
    }

    bool FileSystem::MoveDir(const std::string& src, const std::string& dst) {
        RK_WARN(File, "Move Dir Not Supported");
        return false;
    }

    bool FileSystem::RenameDir(const std::string& src, const std::string& dst) {
        RK_WARN(File, "Rename Dir Not Supported");
        return false;
    }

    bool FileSystem::CopyDir(const std::string& src, const std::string& dst) {
        RK_WARN(File, "Copy Dir Not Supported");
        return false;
    }
}
