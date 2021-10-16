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
        FileBuffer buffer;
        FileBuffer buffer_as;
        FileSystem::LoadSync(root, name, FileOperateMode::READ_TEXT, buffer);
        //auto file = FileSystem::LoadSync(root, name, FileOperateMode::READ_TEXT);

        std::cout << "Read Data[0]: " << ((char*)buffer.buffer)[0] << std::endl;
        std::cout << "Read Data: " << (char*)buffer.buffer << std::endl;
    }

    {
        std::string name = "_root_dir_.zip";
        auto file = FileSystem::OpenZip(root, name, FileOperateMode::READ_BINARY);
        FileBuffer buffer;
        file->ReadFile(buffer, "_root_dir_");
        printf("Read Data: ");
        for(int i = 0; i < buffer.size; ++i) {
            printf("%c", ((char*)buffer.buffer)[i]);
        }
        printf("\n");
        FileSystem::CloseZip(std::move(file));
    }

    Rocket::Log::End();
    return 0;
}