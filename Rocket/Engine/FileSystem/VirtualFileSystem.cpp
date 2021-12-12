#include "FileSystem/VirtualFileSystem.h"

namespace Rocket {
    FilePtr VirtualFileSystem::OpenFile(const std::string& filePath, FileMode::FileOperate mode) {
        return nullptr;
    }

    void VirtualFileSystem::CloseFile(FilePtr file) {
    }
}
