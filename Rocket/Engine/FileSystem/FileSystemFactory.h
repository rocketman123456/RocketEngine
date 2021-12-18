#pragma once
#include "Basic/FileSystem.h"

#include <unordered_map>
#include <functional>

namespace Rocket {
    enum class FileSystemType : int32_t {
        Native = 0,
        Memory,
        Compressed,
    };

    using FileSystemCreator = std::function<FileSystemPtr(const std::string&)>;

    class FileSystemFactory {
    public:
        static void RegisterFileSystem(FileSystemType type, const FileSystemCreator& creator);
        static void UnregisterFileSystem(FileSystemType type);
        static FileSystemPtr CreateFileSystem(FileSystemType type, const std::string& path);
    private:
        static std::unordered_map<FileSystemType, FileSystemCreator> file_systems;
    };
}
