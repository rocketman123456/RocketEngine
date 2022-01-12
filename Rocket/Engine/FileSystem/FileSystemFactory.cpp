#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/ZipFile/ZipFileSystem.h"

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
        // build-in filesystem
        switch(type) {
            case FileSystemType::Memory: {
                return std::make_shared<MemoryFileSystem>(rpath, vpath);
            };
            case FileSystemType::Zip: {
                return std::make_shared<ZipFileSystem>(rpath, vpath);
            };
            case FileSystemType::Native: {
                return std::make_shared<NativeFileSystem>(rpath, vpath);
            };
        }
        // self-defined filesystem
        auto found = file_systems.find(type);
        if(found != file_systems.end()) {
            return found->second(rpath, vpath);
        } else {
            return nullptr;
        }
    }
}
