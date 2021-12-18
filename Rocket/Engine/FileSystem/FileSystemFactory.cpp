#include "FileSystem/FileSystemFactory.h"

namespace Rocket {
    std::unordered_map<FileSystemType, FileSystemCreator> FileSystemFactory::file_systems = {};

    void FileSystemFactory::RegisterFileSystem(FileSystemType type, const FileSystemCreator& creator) {
    }

    void FileSystemFactory::UnregisterFileSystem(FileSystemType type) {
    }

    FileSystemPtr FileSystemFactory::CreateFileSystem(FileSystemType type, const std::string& path) {
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
