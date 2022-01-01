#include "FileSystem/VirtualFileSystem.h"
#include "FileSystem/FileSystemFactory.h"
#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "FileSystem/MemoryFile/MemoryFileSystem.h"
#include "FileSystem/ZipFile/ZipFileSystem.h"

#include "Utils/FindRootDir.h"
#include "Utils/PrintFiles.h"
#include "Utils/GetTime.h"
#include "Log/Instrumentor.h"

#include <iostream>
#include <string>
#include <cassert>

using namespace Rocket;

int main() {
    RK_PROFILE_BEGIN_SESSION("FileSystem", "FileSystem-Profile.json");

    {
        RK_PROFILE_SCOPE("Register");
        FileSystemFactory::RegisterFileSystem(FileSystemType::Native, 
            [](const std::string& rpath, const std::string& vpath){
                return std::make_shared<NativeFileSystem>(rpath, vpath);
            }
        );
        FileSystemFactory::RegisterFileSystem(FileSystemType::Memory, 
            [](const std::string& rpath, const std::string& vpath){
                return std::make_shared<MemoryFileSystem>(rpath, vpath);
            }
        );
        FileSystemFactory::RegisterFileSystem(FileSystemType::Zip, 
            [](const std::string& rpath, const std::string& vpath){
                return std::make_shared<ZipFileSystem>(rpath, vpath);
            }
        );
    }

    VirtualFileSystemPtr vfs = std::make_shared<VirtualFileSystem>();

    std::string root = FindRootDir("_root_dir_");
    std::string asset_path = root + "/Asset";
    std::string zip_path = root + "/Asset/Music.zip";
    std::string vpath_native = "/Asset";
    std::string vpath_zip = "/Asset/Music";
    std::string vpath_memory = "/Asset/Memory";

    {
        // From Profile, this Mount function will take longest time,
        // should use only at program startup
        RK_PROFILE_SCOPE("Mount");
        auto nfs = FileSystemFactory::CreateFileSystem(FileSystemType::Native, asset_path, vpath_native);
        vfs->MountFileSystem(nfs, vpath_native);
        auto zfs = FileSystemFactory::CreateFileSystem(FileSystemType::Zip, zip_path, vpath_zip);
        vfs->MountFileSystem(zfs, vpath_zip);
        auto mfs = FileSystemFactory::CreateFileSystem(FileSystemType::Memory, "/", vpath_memory);
        vfs->MountFileSystem(mfs, vpath_memory);
    }

    {
        std::string obj_file = "/Asset/Model/cube/cube.obj";
        std::string wav_file = "/Asset/Music/Piano.ff.A0.wav";
        RK_PROFILE_SCOPE("ReadWrite");
        auto native_file = vfs->GetFilePointer(obj_file);
        //auto zip_file = vfs->GetFilePointer("/Zip/Config/basic.yaml");
        auto zip_file = vfs->GetFilePointer(wav_file);

        FileBuffer buffer;
        FileBuffer buffer_zip;

        vfs->OpenFile(native_file, FileEnum::READWRITE_TEXT);
        {
            RK_PROFILE_SCOPE("ReadNative");
            // FileBuffer buffer = {(FileByte*)alloca(native_file->Size()+1), native_file->Size()+1};
            buffer = {new FileByte[native_file->Size()+1], native_file->Size()+1};
            auto result = vfs->ReadFile(native_file, &buffer);
            std::cout << "Read: " << obj_file << " Size: " << result << std::endl;
            //std::cout << "Content: " << std::endl;
            //std::cout << reinterpret_cast<char*>(buffer.data()) << std::endl;
            //delete [] buffer.data();
        }
        vfs->CloseFile(native_file);

        // Able to read file from mix file system structure
        vfs->OpenFile(zip_file, FileEnum::READWRITE_BINARY);
        {
            RK_PROFILE_SCOPE("ReadZip");
            // FileBuffer buffer = {(FileByte*)alloca(zip_file->Size()), zip_file->Size()};
            buffer_zip = {new FileByte[zip_file->Size()], zip_file->Size()};
            auto result = vfs->ReadFile(zip_file, &buffer_zip);
            //std::string buffer_str((char*)buffer.data(), buffer.size());
            std::cout << "Read: " << wav_file << " Size: " << result << std::endl;
            //std::string content = "root dir - zip test: " + CurrentDate();
            //auto size = vfs->WriteFile(zip_file, {(std::byte*)content.data(), content.size()});
            //assert(size == content.size());
            // delete [] buffer_zip.data();
        }
        vfs->CloseFile(zip_file);

        std::string memory_file = "/Asset/Memory/Config.txt";
        {
            RK_PROFILE_SCOPE("CreateMemoryFile");
            int result = vfs->CreateFile(memory_file);
            std::cout << "Create " << memory_file << " Result " << result << std::endl;
            auto mem_file = vfs->GetFilePointer(memory_file);
            vfs->OpenFile(mem_file, FileEnum::READWRITE_BINARY);
            vfs->WriteFile(mem_file, buffer_zip);
            vfs->CloseFile(mem_file);
        }

        {
            std::string native_file = "/Asset/Config/Special/Config.txt";
            RK_PROFILE_SCOPE("CreateNativeFile");
            int result = vfs->CreateFile(native_file);
            std::cout << "Create " << native_file << " Result " << result << std::endl;
            auto nat_file = vfs->GetFilePointer(native_file);
            vfs->OpenFile(nat_file, FileEnum::READWRITE_BINARY);
            vfs->WriteFile(nat_file, buffer_zip);
            vfs->CloseFile(nat_file);
        }

        {
            RK_PROFILE_SCOPE("ReadMemoryFile");
            auto mem_file = vfs->GetFilePointer(memory_file);
            FileBuffer buffer_mem = {new FileByte[mem_file->Size()], mem_file->Size()};
            vfs->OpenFile(mem_file, FileEnum::READWRITE_BINARY);
            auto result = vfs->ReadFile(mem_file, &buffer_mem);
            std::cout << "Read: " << memory_file << " Size: " << result << std::endl;
            vfs->CloseFile(mem_file);
            delete [] buffer_mem.data();
        }

        if(buffer.data() != nullptr)
            delete [] buffer.data();
        if(buffer_zip.data() != nullptr)
            delete [] buffer_zip.data();
    }

    {
        RK_PROFILE_SCOPE("Unmount");
        vfs->UnmountFileSystem(vpath_native);
        vfs->UnmountFileSystem(vpath_zip);
    }

    RK_PROFILE_END_SESSION("FileSystem");
    return 0;
}
