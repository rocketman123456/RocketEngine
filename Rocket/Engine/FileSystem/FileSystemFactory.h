#pragma once
#include "Basic/FileSystem.h"

#include <unordered_map>
#include <functional>

namespace Rocket {
    enum class FileSystemType : int32_t {
        Native = 0,
        Memory,
        Zip,
    };

    using FileSystemCreator = std::function<FileSystemPtr(const std::string&, const std::string&)>;

    class FileSystemFactory {
    public:
        static void RegisterFileSystem(FileSystemType type, const FileSystemCreator& creator);
        static void UnregisterFileSystem(FileSystemType type);
        static FileSystemPtr CreateFileSystem(FileSystemType type, const std::string& rpath, const std::string& vpath);
    private:
        static std::unordered_map<FileSystemType, FileSystemCreator> file_systems;
    };
}
