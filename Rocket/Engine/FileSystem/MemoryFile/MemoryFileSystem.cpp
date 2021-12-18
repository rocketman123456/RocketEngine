#include "FileSystem/MemoryFile/MemoryFileSystem.h"

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

    FilePtr MemoryFileSystem::GetFilePointer(const std::string& file_path) {
        if(!IsFileExists(file_path)) {
            RK_WARN(File, "File Not Exist {}", file_path);
            return nullptr;
        }
        // TODO : make a usable memory file storage
        auto full_path = file_path.substr(virtual_path.size());
        auto file = std::make_shared<MemoryFile>(full_path, file_path);
        return file;
    }
}
