#include "FileSystem/VirtualFileSystem.h"
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
    std::string asset_path = root + "/Asset";
    NativeFileSystemPtr nfs = std::make_shared<NativeFileSystem>(asset_path, "/Native");

    nfs->Initialize();

    // DisplayNativeDirTree(nfs->RealPath());
    auto native_file = nfs->OpenFile("/Native/Config/music_name.txt", FileEnum::READWRITE_BINARY);
    nfs->CloseFile(native_file);

    nfs->Finalize();

    ZipFileSystemPtr zfs = std::make_shared<ZipFileSystem>(root + "/_root_dir_.zip", "/Zip");
    zfs->Initialize();

    auto zip_file = zfs->OpenFile("/Zip/Config/basic.yaml", FileEnum::READWRITE_BINARY);
    FileBuffer buffer = {new std::byte[zip_file->Size()], zip_file->Size()};
    zfs->ReadFile(zip_file, &buffer);
    std::string buffer_str((char*)buffer.data(), buffer.size());
    std::cout << buffer_str << std::endl;
    zfs->CloseFile(zip_file);

    zfs->Finalize();

    return 0;
}
