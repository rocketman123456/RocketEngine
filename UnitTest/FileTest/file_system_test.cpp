#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/ZipFile/ZipFileSystem.h"

#include "Utils/FindRootDir.h"
#include "Utils/PrintFiles.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace Rocket;

int main() {
    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset/Model";
    std::string zip_path = root + "/_root_dir_.zip";

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

    {
        std::string vpath = "/Native";
        auto nfs = FileSystemFactory::CreateFileSystem(FileSystemType::Native, asset_path, vpath);
        nfs->Initialize();

        vfs->MountFileSystem(nfs, vpath);

        // DisplayNativeDirTree(nfs->RealPath());
        auto native_file = nfs->GetFilePointer("/Native/cube/cube.obj");
        nfs->OpenFile(native_file, FileEnum::READWRITE_BINARY);
        nfs->CloseFile(native_file);

        vfs->UnmountFileSystem(nfs);

        nfs->Finalize();
    }

    {
        std::string vpath = "/Zip";
        auto zfs = FileSystemFactory::CreateFileSystem(FileSystemType::Zip, zip_path, vpath);
        zfs->Initialize();

        vfs->MountFileSystem(zfs, vpath);

        auto zip_file = zfs->GetFilePointer("/Zip/Config/basic.yaml");
        zfs->OpenFile(zip_file, FileEnum::READWRITE_BINARY);
        FileBuffer buffer = {new std::byte[zip_file->Size()], zip_file->Size()};
        zfs->ReadFile(zip_file, &buffer);
        std::string buffer_str((char*)buffer.data(), buffer.size());
        std::cout << buffer_str << std::endl;
        zfs->CloseFile(zip_file);

        vfs->UnmountFileSystem(vpath);

        zfs->Finalize();
    }

    return 0;
}
