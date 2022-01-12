#pragma once
#include "FileSystem/VirtualFileSystem.h"

#include <string>

namespace Rocket {
    std::string ReadShaderFile(const std::string& root, const std::string& name);
    std::string ReadShaderFile(const std::string& name, VirtualFileSystemPtr vfs);
}
