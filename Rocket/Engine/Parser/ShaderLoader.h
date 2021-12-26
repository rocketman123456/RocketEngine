#pragma once
#include "FileSystem/VirtualFileSystem.h"

#include <string>

namespace Rocket {
    std::string ShaderLoader(VirtualFileSystemPtr& vfs, const std::string& name);
}
