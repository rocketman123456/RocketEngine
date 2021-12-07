#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "Utils/FindRootDir.h"

#include <iostream>

using namespace Rocket;

int main() {
    std::string root = FindRootDir("_root_dir_");
    NativeFileSystemPtr nfs = std::make_shared<NativeFileSystem>(root);
    //NativeFileSystemPtr nfs = std::make_shared<NativeFileSystem>(root + "/Asset/Config");
    nfs->Initialize();
    std::cout << "NFS: " << nfs->Files().size() << std::endl;

    //auto info = std::make_shared<FileInfo>("music_name.txt");
    auto info = std::make_shared<FileInfo>("_root_dir_");
    auto file = nfs->OpenFile(info, FileMode::READ_BINARY);
    if(file != nullptr) {
        //file->Open(FileMode::READ_BINARY);
        gsl::span<gsl::byte> data = {new gsl::byte[file->Size()], file->Size()};
        file->Read(data);
        std::cout << data.size() << "," << (char*) data.data() << std::endl;
        delete [] data.data();
    }

    nfs->Finalize();
    VirtualFileSystem vfs;
    return 0;
}
