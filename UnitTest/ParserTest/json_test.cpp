#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/ZipFile/ZipFileSystem.h"
#include "Parser/JsonParser.h"
#include "Utils/FindRootDir.h"
#include "Log/Log.h"

#include <iostream>

using namespace Rocket;

int main() {
    {
        FileSystemFactory::RegisterFileSystem(FileSystemType::Native, 
            [](const std::string& rpath, const std::string& vpath){ return std::make_shared<NativeFileSystem>(rpath, vpath); }
        );
        FileSystemFactory::RegisterFileSystem(FileSystemType::Memory, 
            [](const std::string& rpath, const std::string& vpath){ return std::make_shared<MemoryFileSystem>(rpath, vpath); }
        );
        FileSystemFactory::RegisterFileSystem(FileSystemType::Zip, 
            [](const std::string& rpath, const std::string& vpath){ return std::make_shared<ZipFileSystem>(rpath, vpath); }
        );
    }

    VirtualFileSystemPtr vfs = std::make_shared<VirtualFileSystem>();

    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset";
    // std::string zip_path = root + "/Asset/Music.zip";
    std::string vpath_native = "/Asset";
    // std::string vpath_zip = "/Asset/Music";
    // std::string vpath_memory = "/Asset/Memory";

    {
        // From Profile, this Mount function will take longest time,
        // should use only at program startup
        auto nfs = FileSystemFactory::CreateFileSystem(FileSystemType::Native, asset_path, vpath_native);
        vfs->MountFileSystem(nfs, vpath_native);
        // auto zfs = FileSystemFactory::CreateFileSystem(FileSystemType::Zip, zip_path, vpath_zip);
        // vfs->MountFileSystem(zfs, vpath_zip);
        // auto mfs = FileSystemFactory::CreateFileSystem(FileSystemType::Memory, "/", vpath_memory);
        // vfs->MountFileSystem(mfs, vpath_memory);
    }

    JsonParser parser;

    parser.Initialize("/Asset/Note/Castle In The Sky 3.json", vfs);
    
    std::cout << parser.Deserialize() << std::endl;

    return 0;
}