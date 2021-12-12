#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
// #include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "Utils/FindRootDir.h"
#include "Utils/PrintFiles.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace Rocket;

int main() {
    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset";
    NativeFileSystemPtr nfs = std::make_shared<NativeFileSystem>(asset_path, "/");
    nfs->Initialize();

    DisplayDirTree(nfs->BasicPath());

    std::string find_file = "/Config/music_name.txt";
    bool result = nfs->IsFileExists(find_file);
    assert(result == true);
    RK_INFO(App, "File Exist: {}, {}", find_file, result);
    std::string find_dir = "/Model/bunny";
    result = nfs->IsDirExists(find_dir);
    assert(result == true);
    RK_INFO(App, "Dir Exist: {}, {}", find_dir, result);
    find_dir = "/Model/bunny_spot";
    result = nfs->IsDirExists(find_dir);
    assert(result == false);
    RK_INFO(App, "Dir Exist: {}, {}", find_dir, result);

    auto file = nfs->OpenFile(find_file, FileMode::READWRITE_BINARY);
    gsl::span<gsl::byte> data = {new gsl::byte[file->Size()], file->Size()};
    file->Read(data);
    std::string content = "";
    for(auto byte : data) {
        content += (char)byte;
    }
    RK_INFO(App, "Content: {}", content);
    nfs->CloseFile(file);

    nfs->Finalize();
    return 0;
}
