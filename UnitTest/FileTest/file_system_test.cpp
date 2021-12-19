#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/ZipFile/ZipFileSystem.h"

#include "Utils/FindRootDir.h"
#include "Utils/PrintFiles.h"
#include "Utils/GetTime.h"
#include "Log/Instrumentor.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace Rocket;

int main() {
    RK_PROFILE_BEGIN_SESSION("FileSystem", "FileSystem-Profile.json");
    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset/Model";
    std::string zip_path = root + "/_root_dir_.zip";
    std::string vpath_native = "/Native";
    std::string vpath_zip = "/Zip";

    {
        RK_PROFILE_SCOPE("Register");
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
        // From Profile, this Mount function will take longest time,
        // should use only at program startup
        RK_PROFILE_SCOPE("Mount");
        auto nfs = FileSystemFactory::CreateFileSystem(FileSystemType::Native, asset_path, vpath_native);
        vfs->MountFileSystem(nfs, vpath_native);
        auto zfs = FileSystemFactory::CreateFileSystem(FileSystemType::Zip, zip_path, vpath_zip);
        vfs->MountFileSystem(zfs, vpath_zip);
    }

    {
        RK_PROFILE_SCOPE("ReadWrite");
        auto native_file = vfs->GetFilePointer("/Native/cube/cube.obj");
        //auto zip_file = vfs->GetFilePointer("/Zip/Config/basic.yaml");
        auto zip_file = vfs->GetFilePointer("/Zip/_root_dir_");

        vfs->OpenFile(native_file, FileEnum::READWRITE_BINARY);
        vfs->CloseFile(native_file);

        vfs->OpenFile(zip_file, FileEnum::READWRITE_BINARY);
        FileBuffer buffer = {new std::byte[zip_file->Size()], zip_file->Size()};
        vfs->ReadFile(zip_file, &buffer);
        std::string buffer_str((char*)buffer.data(), buffer.size());
        std::cout << buffer_str << std::endl;
        std::string content = "root dir - zip test: " + CurrentDate();
        auto size = vfs->WriteFile(zip_file, {(std::byte*)content.data(), content.size()});
        assert(size == content.size());
        vfs->CloseFile(zip_file);
    }

    {
        RK_PROFILE_SCOPE("Unmount");
        vfs->UnmountFileSystem(vpath_native);
        vfs->UnmountFileSystem(vpath_zip);
    }

    RK_PROFILE_END_SESSION("FileSystem");
    return 0;
}
