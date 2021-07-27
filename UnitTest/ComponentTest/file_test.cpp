#include "Config/ConfigSetting.h"
#include "FileSystem/FileSystem.h"
#include "Utils/FindRootDir.h"
#include "Log/Log.h"

#include <iostream>

using namespace Rocket;

int main() {
    Rocket::Log::Init();

    std::string name = "_root_dir_";
    auto root = FindRootDir(name);
    //std::cout << root << std::endl;

    root += "/";

    //auto file = FileSystem::OpenSync(root, name, FileOperateMode::ReadBinary);
    auto file = FileSystem::OpenSync(root, name, FileOperateMode::ReadText);
    FileBuffer buffer;
    file->ReadAll(buffer);

    std::cout << "Read Data[0]: " << ((char*)buffer.buffer)[0] << std::endl;
    std::cout << "Read Data: " << (char*)buffer.buffer << std::endl;

    // must have memory manager
    //delete[] (char*)buffer.buffer;

    return 0;
}