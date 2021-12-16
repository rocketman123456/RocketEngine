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

    class FileSystemFactory {
    public:
        static void RegisterFileSystem();
        static void UnregisterFileSystem();
        static FileSystemPtr CreateFileSystem(FileSystemType type);
    private:
        static std::unordered_map<FileSystemType, std::function<FileSystemPtr()> > file_systems;
    };
}
