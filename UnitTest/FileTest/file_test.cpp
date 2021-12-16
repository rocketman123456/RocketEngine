#include "FileSystem/NativeFile/NativeFile.h"
#include "FileSystem/MemoryFile/MemoryFile.h"
#include "Utils/FindRootDir.h"

#include <iostream>

using namespace Rocket;

int main() {
    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset";
    NativeFilePtr nf = std::make_shared<NativeFile>(root + "/" + "_root_dir_");
    MemoryFilePtr mf = std::make_shared<MemoryFile>("/_memory_root_");

    nf->Open(FileEnum::READWRITE_BINARY);
    FileBuffer data = {new FileByte[nf->Size()], nf->Size()};
    nf->Read(data);

    {
        std::string data_str((char*)data.data(), data.size());
        std::cout << data_str << std::endl;
    }

    {
        mf->Open(FileEnum::READWRITE_BINARY);
        mf->Write(data);
        FileBuffer data_copy = {new FileByte[mf->Size()], mf->Size()};
        mf->Read(data_copy);
        std::string data_str((char*)data_copy.data(), data_copy.size());
        std::cout << data_str << std::endl;
    }

    return 0;
}