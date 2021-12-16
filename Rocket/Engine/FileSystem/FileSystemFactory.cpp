#include "FileSystem/FileSystemFactory.h"

namespace Rocket {
    std::unordered_map<FileSystemType, std::function<FileSystemPtr()> > FileSystemFactory::file_systems = {};

    void FileSystemFactory::RegisterFileSystem() {
    }

    void FileSystemFactory::UnregisterFileSystem() {
    }

    FileSystemPtr FileSystemFactory::CreateFileSystem(FileSystemType type) {
        switch (type) {
        case FileSystemType::Native:
            return nullptr;
        case FileSystemType::Memory:
            return nullptr;
        case FileSystemType::Compressed:
            return nullptr;
        default:
            return nullptr;
        }
    }
}
