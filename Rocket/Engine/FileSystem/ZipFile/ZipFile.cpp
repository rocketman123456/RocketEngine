#include "FileSystem/ZipFile/ZipFile.h"

namespace Rocket {
    ZipFile::ZipFile(const VirtualNodePtr& vnode_) : vnode(vnode_) {
        // TODO
        virtual_path = vnode->path + vnode->name;
    }
    ZipFile::ZipFile(const std::string& path_, const std::string& virtual_path_)
        : real_path(path_), virtual_path(virtual_path_) {}
    ZipFile::ZipFile(const std::string& path_)
        : real_path(path_) {}

    void Open(int32_t mode) {
        
    }

    void Close() {
        
    }

    std::size_t Seek(std::size_t offset, FileEnum::Origin origin) {
        return 0;
    }

    std::size_t Tell() {
        return 0;
    }

    std::size_t Read(FileBuffer& data) {
        return 0;
    }

    std::size_t Write(const FileBuffer& data) {
        return 0;
    }
}