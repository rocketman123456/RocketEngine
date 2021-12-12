#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
// #include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "Utils/FindRootDir.h"
#include "Utils/PrintFiles.h"

#include <iostream>
#include <string>

using namespace Rocket;

int main() {
    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset";
    NativeFileSystemPtr nfs = std::make_shared<NativeFileSystem>(asset_path);
    nfs->Initialize();
    DisplayDirTree(nfs->BasicPath());

    {
        //auto info = std::make_shared<FileInfo>("music_name.txt");
        // auto info = std::make_shared<FileInfo>("_root_dir_");
        // auto file = nfs->OpenFile(info, FileMode::READWRITE_BINARY | FileMode::APPEND);
        // auto mem_info = std::make_shared<FileInfo>("memory_file_test");
        // auto mem_file = mfs->OpenFile(info, FileMode::READ_BINARY);
        // if(file != nullptr) {
        //     gsl::span<gsl::byte> data = {new gsl::byte[file->Size()], file->Size()};
        //     file->Read(data);

        //     std::string data_str = "";
        //     for(int i = 0; i < data.size(); ++i) {
        //         data_str += (char)(((char*)data.data())[i]);
        //     }
        //     std::cout << data.size() << "," << data_str << std::endl;

        //     if(mem_file != nullptr) {
        //         mem_file->Write(data);
        //     }

        //     gsl::span<gsl::byte> mem_data = {new gsl::byte[data.size()], data.size()};
        //     mem_file->Read(mem_data);
        //     std::string mem_data_str = "";
        //     for(int i = 0; i < mem_data.size(); ++i) {
        //         mem_data_str += (char)(((char*)data.data())[i]);
        //     }
        //     std::cout << mem_data.size() << "," << mem_data_str << std::endl;

        //     //file->Write(data);
        //     delete [] data.data();
        // }
        //auto src_info = std::make_shared<FileInfo>("_root_dir_");
        //auto dst_info = std::make_shared<FileInfo>("_root_dir_.txt");
        //nfs->CopyFile(src_info, dst_info);
    }

    nfs->Finalize();
    return 0;
}
