#include "Utils/FindRootDir.h"
#include "FileSystem/ZipFile/ZipUtils.h"
#include "FileSystem/ZipFile/ZipFile.h"
#include "Log/Log.h"

#include <zip.h>
#include <iostream>
#include <cassert>
#include <time.h>

using namespace Rocket;

std::string current_date(){ 
    time_t now = time(NULL); 
    struct tm tstruct; 
    char buf[40]; 
    tstruct = *localtime(&now); 
    //format: day DD-MM-YYYY 
    strftime(buf, sizeof(buf), "%A %d/%m/%Y", &tstruct); 
    return buf; 
}

std::string current_time(){ 
    time_t now = time(NULL); 
    struct tm tstruct; 
    char buf[40]; 
    tstruct = *localtime(&now); 
    //format: HH:mm:ss 
    strftime(buf, sizeof(buf), "%X", &tstruct); 
    return buf; 
}

int main() {
    std::string root = FindRootDir("_root_dir_");
    std::string zip_name = "_root_dir_.zip";
    std::string full_name = root + "/" + zip_name;

    int err;
    zip_t* zip_archive;
    zip_archive = zip_open(full_name.c_str(), ZIP_CREATE, &err);
    if(zip_archive == nullptr) {
        char buf[100];
        zip_error_to_str(buf, sizeof(buf), err, errno);
        RK_ERROR(File, "Unable to open zip: {}, Error: {}", full_name, buf);
    } else {
        RK_INFO(File, "Open zip success: {}", full_name);
    }

    std::string zip_file_name = "_root_dir_";
    std::string content = "root dir - zip test: " + current_date();

    {
        auto zip_file = OpenZipFile(zip_archive, zip_file_name);
        if(zip_file == nullptr) {
            RK_ERROR(File, "Unable to open file: {}", zip_file_name);
        } else {
            RK_INFO(File, "Open file success: {}", zip_file_name);
        }
        zip_stat_t zip_file_status;
        zip_stat(zip_archive, zip_file_name.c_str(), 0, &zip_file_status);
        CloseZipFile(zip_file);
    }

    {
        ZipFilePtr zfp = std::make_shared<ZipFile>(zip_file_name, zip_archive);
        zfp->Open(FileEnum::READWRITE_BINARY);

        FileBuffer data = {new FileByte[zfp->Size()], zfp->Size()};
        zfp->Read(data);
        std::string data_str((char*)data.data(), data.size());
        std::cout << data_str << std::endl;
        delete [] data.data();

        auto size = zfp->Write({(std::byte*)content.data(), content.size()});
        assert(size == content.size());

        zfp->Close();
    }

    // zip_source_t* source;
    // source = zip_source_buffer(zip_archive, content.data(), content.size(), 0);
    // if(source == nullptr) {
    //     RK_WARN(File, "error adding in zip file: {}", zip_strerror(zip_archive));
    //     return 0;
    // }

    // auto result = zip_file_add(zip_archive, zip_file_name.c_str(), source, ZIP_FL_OVERWRITE);
    // if (result < 0) {
    //     zip_source_free(source);
    //     RK_WARN(File, "error adding in zip file: {}", zip_strerror(zip_archive));
    // }

    {
        auto result = zip_dir_add(zip_archive, "Asset", 0);
        if(result < 0) {
            std::cout << zip_strerror(zip_archive) << std::endl;
        }
    }

    {
        auto result = zip_close(zip_archive);
        if(result < 0) {
            std::cout << zip_strerror(zip_archive) << std::endl;
        }
    }

    return 0;
}