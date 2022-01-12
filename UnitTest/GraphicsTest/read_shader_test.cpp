#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/ZipFile/ZipFileSystem.h"
#include "Parser/ShaderParser.h"
#include "Utils/FindRootDir.h"

#include <iostream>

using namespace Rocket;

int main() {
    VirtualFileSystemPtr vfs = std::make_shared<VirtualFileSystem>();

    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset";
    std::string vpath_native = "/Asset";

    auto nfs = FileSystemFactory::CreateFileSystem(FileSystemType::Native, asset_path, vpath_native);
    vfs->MountFileSystem(nfs, vpath_native);

    auto code = ReadShaderFile("/Asset/Shader/shader_depth.vert", vfs);

    std::cout << code << std::endl;

    return 0;
}