#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/ZipFile/ZipFileSystem.h"
#include "Common/Utils/ShaderFileUtils.h"
#include "Utils/FindRootDir.h"

#include <iostream>

using namespace Rocket;

int main() {
    {
        FileSystemFactory::RegisterFileSystem(FileSystemType::Native, 
            [](const std::string& rpath, const std::string& vpath){
                return std::make_shared<NativeFileSystem>(rpath, vpath);
            }
        );
        FileSystemFactory::RegisterFileSystem(FileSystemType::Memory, 
            [](const std::string& rpath, const std::string& vpath){
                return std::make_shared<MemoryFileSystem>(rpath, vpath);
            }
        );
        FileSystemFactory::RegisterFileSystem(FileSystemType::Zip, 
            [](const std::string& rpath, const std::string& vpath){
                return std::make_shared<ZipFileSystem>(rpath, vpath);
            }
        );
    }

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