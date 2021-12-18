#include "FileSystem/Basic/FileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"
#include "Utils/StringUtils.h"

namespace Rocket {
    FileSystem::FileSystem(const std::string& real_path, const std::string& virtual_path)
        : real_path(real_path), virtual_path(virtual_path) {}

    VNodeList FileSystem::VNodes(const std::string& dir) const {
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

    VBlockList FileSystem::VBlocks(const std::string& dir) const {
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

    bool FileSystem::IsFileExists(const std::string& file_path) const {
        auto found = node_map.find(file_path);
        if(found == node_map.end())
            return false;
        else
            return true;
    }

    bool FileSystem::IsDirExists(const std::string& dir_path) const {
        auto found = block_map.find(dir_path);
        if(found == block_map.end())
            return false;
        else
            return true;
    }

    bool FileSystem::IsFile(const std::string& file_path) const { return IsFileExists(file_path); }
    bool FileSystem::IsDir(const std::string& dir_path) const { return IsDirExists(dir_path); }
    void FileSystem::OpenFile(const FilePtr& file, int32_t mode) { file->Open(mode); }
    void FileSystem::CloseFile(const FilePtr& file) { file->Close(); }
    std::size_t FileSystem::ReadFile(const FilePtr& file, FileBuffer* data) { return file->Read(data); }
    std::size_t FileSystem::WriteFile(FilePtr& file, const FileBuffer& data) { return file->Write(data); }

    bool FileSystem::CreateFile(const std::string& file_path) {
        RK_WARN(File, "Create File Not Supported");
        return false;
    }

    bool FileSystem::RemoveFile(const std::string& file_path) {
        RK_WARN(File, "Remove File Not Supported");
        return false;
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

    bool FileSystem::CreateDir(const std::string& dir_path) {
        RK_WARN(File, "Create Dir Not Supported");
        return false;
    }

    bool FileSystem::RemoveDir(const std::string& dir_path) {
        RK_WARN(File, "Remove Dir Not Supported");
        return false;
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
