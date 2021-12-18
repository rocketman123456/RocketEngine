#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/Basic/VirtualUtils.h"

namespace Rocket {
    MemoryFileSystem::MemoryFileSystem(const std::string& real_path) 
        : real_path(real_path), virtual_path("/") {}
    MemoryFileSystem::MemoryFileSystem(const std::string& real_path, const std::string& virtual_path)
        : real_path(real_path), virtual_path(virtual_path) {}

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
        node_map.clear();
        block_map.clear();
        is_initialized = false;
    }

    void MemoryFileSystem::SetVirtualPath(const std::string& vpath) {
        virtual_path = vpath;
    }

    void MemoryFileSystem::SetRealPath(const std::string& rpath) {
        real_path = rpath;
    }

    void MemoryFileSystem::CreateVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level) {
        if(root == nullptr || dirs.size() == 0 || level == dirs.size()) return;
        return;
    }

    VNodeList MemoryFileSystem::VNodes(const std::string& dir) const {
        auto dir_ = Replace(virtual_path, "\\", "/");
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

    VBlockList MemoryFileSystem::VBlocks(const std::string& dir) const {
        auto dir_ = Replace(virtual_path, "\\", "/");
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

    bool MemoryFileSystem::IsFileExists(const std::string& file_path) const {
        auto found = node_map.find(file_path);
        if(found == node_map.end())
            return false;
        else
            return true;
    }

    bool MemoryFileSystem::IsDirExists(const std::string& dir_path) const {
        auto found = block_map.find(dir_path);
        if(found == block_map.end())
            return false;
        else
            return true;
    }

    bool MemoryFileSystem::IsFile(const std::string& file_path) const {
        return IsFileExists(file_path);
    }

    bool MemoryFileSystem::IsDir(const std::string& dir_path) const {
        return IsDirExists(dir_path);
    }

    bool MemoryFileSystem::IsReadOnly() const {
        return false;
    }

    FilePtr MemoryFileSystem::OpenFile(const std::string& file_path, int32_t mode) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        // TODO : make a usable memory file storage
        auto full_path = file_path.substr(virtual_path.size());
        auto file = std::make_shared<MemoryFile>(full_path, file_path);
        file->Open(mode);
        return file;
    }

    void MemoryFileSystem::CloseFile(const FilePtr& file) {
        file->Close();
    }

    std::size_t MemoryFileSystem::ReadFile(const FilePtr& file, FileBuffer* data) {
        return file->Read(data);
    }

    std::size_t MemoryFileSystem::WriteFile(FilePtr& file, const FileBuffer& data) {
        return file->Write(data);
    }

    bool MemoryFileSystem::CreateFile(const std::string& file_path) {
        RK_WARN(File, "Create File Not Supported");
        return false;
    }

    bool MemoryFileSystem::RemoveFile(const std::string& file_path) {
        RK_WARN(File, "Remove File Not Supported");
        return false;
    }

    std::size_t MemoryFileSystem::FileSize(const FilePtr& file) const {
        return file->Size();
    }

    bool MemoryFileSystem::CreateDir(const std::string& dir_path) {
        RK_WARN(File, "Create Dir Not Supported");
        return false;
    }

    bool MemoryFileSystem::RemoveDir(const std::string& dir_path) {
        RK_WARN(File, "Remove Dir Not Supported");
        return false;
    }
}
