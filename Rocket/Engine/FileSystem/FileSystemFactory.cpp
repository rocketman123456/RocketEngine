#include "FileSystem/FileSystemFactory.h"

namespace Rocket {
    std::unordered_map<FileSystemType, FileSystemCreator> FileSystemFactory::file_systems = {};

    void FileSystemFactory::RegisterFileSystem(FileSystemType type, const FileSystemCreator& creator) {
        file_systems[type] = creator;
    }

    void FileSystemFactory::UnregisterFileSystem(FileSystemType type) {
        auto found = file_systems.find(type);
        if(found != file_systems.end()) {
            file_systems.erase(found);
        }
    }

    FileSystemPtr FileSystemFactory::CreateFileSystem(FileSystemType type, const std::string& rpath, const std::string& vpath) {
        auto found = file_systems.find(type);
        if(found != file_systems.end()) {
            return found->second(rpath, vpath);
        } else {
            return nullptr;
        }
    }
}
