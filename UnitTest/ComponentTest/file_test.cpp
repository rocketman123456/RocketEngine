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
    root += "/";

    {
        std::string name = "_root_dir_";
        //auto file = FileSystem::OpenSync(root, name, FileOperateMode::ReadBinary);
        auto file = FileSystem::OpenSync(root, name, FileOperateMode::ReadText);
        FileBuffer buffer;
        file->ReadAll(buffer);

        std::cout << "Read Data[0]: " << ((char*)buffer.buffer)[0] << std::endl;
        std::cout << "Read Data: " << (char*)buffer.buffer << std::endl;

        FileSystem::CloseSync(std::move(file));

        // must have memory manager
        //delete[] (char*)buffer.buffer;
    }

    {
        std::string name = "_root_dir_.zip";
        auto file = FileSystem::OpenZip(root, name, FileOperateMode::ReadBinary);
        FileBuffer buffer;
        file->ReadFile(buffer, "_root_dir_");
        printf("Read Data: ");
        for(int i = 0; i < buffer.size; ++i) {
            printf("%c", ((char*)buffer.buffer)[i]);
        }
        printf("\n");
        FileSystem::CloseZip(std::move(file));

        // must have memory manager
        //delete[] (char*)buffer.buffer;
    }

    return 0;
}