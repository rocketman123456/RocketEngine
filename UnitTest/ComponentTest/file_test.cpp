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
        auto file = FileSystem::OpenSync(root, name, FileOperateMode::READ_TEXT);
        //auto file = FileSystem::OpenSync(root, name, FileOperateMode::READ_TEXT);
        auto file_as = FileSystem::OpenAsync(root, name, FileOperateMode::READ_TEXT);
        FileBuffer buffer;
        file->ReadAll(buffer);

        FileBuffer buffer_as;
        auto result = file_as->ReadAllAsync(buffer_as);

        std::cout << "Read Data[0]: " << ((char*)buffer.buffer)[0] << std::endl;
        std::cout << "Read Data: " << (char*)buffer.buffer << std::endl;

        //result.wait();
        auto file_size = result.get();
        std::cout << "Read Async Size: " << file_size << std::endl;
        std::cout << "Read Async Data: " << (char*)buffer_as.buffer << std::endl;

        FileSystem::CloseSync(std::move(file));
        FileSystem::CloseSync(std::move(file_as));
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