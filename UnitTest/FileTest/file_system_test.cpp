#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "Utils/FindRootDir.h"

#include <iostream>

using namespace Rocket;

int main() {
    std::string root = FindRootDir("_root_dir_");
    NativeFileSystemPtr nfs = std::make_shared<NativeFileSystem>(root);
    MemoryFileSystemPtr mfs = std::make_shared<MemoryFileSystem>();
    //NativeFileSystemPtr nfs = std::make_shared<NativeFileSystem>(root + "/Asset/Config");
    nfs->Initialize();
    std::cout << "NFS: " << nfs->Files().size() << std::endl;
    mfs->Initialize();

    {
        //auto info = std::make_shared<FileInfo>("music_name.txt");
        auto info = std::make_shared<FileInfo>("_root_dir_");
        auto file = nfs->OpenFile(info, FileMode::READWRITE_BINARY | FileMode::APPEND);
        if(file != nullptr) {
            gsl::span<gsl::byte> data = {new gsl::byte[file->Size()], file->Size()};
            file->Read(data);
            std::cout << data.size() << ",";
            for(int i = 0; i > data.size(); ++i) {
                printf("%c", ((char*)data.data())[i]);
            }
            std::cout << std::endl;
            //file->Write(data);
            delete [] data.data();
        }
        auto src_info = std::make_shared<FileInfo>("_root_dir_");
        auto dst_info = std::make_shared<FileInfo>("_root_dir_.txt");
        nfs->CopyFile(src_info, dst_info);
    }

    {
        auto info = std::make_shared<FileInfo>("_root_dir_");
        auto file = mfs->OpenFile(info, FileMode::READ_BINARY);
    }

    nfs->Finalize();
    VirtualFileSystem vfs;
    return 0;
}
