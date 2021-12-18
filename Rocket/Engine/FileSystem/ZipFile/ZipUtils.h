#pragma once
#include <zip.h>
#include <string>

namespace Rocket {
    zip_t* OpenZip(const std::string& file_name, uint32_t mode = ZIP_CREATE);
    void CloseZip(zip_t* zip);
    zip_file_t* OpenZipFile(zip_t* zip, const std::string& file_name, uint32_t mode = 0);
    void CloseZipFile(zip_file_t* zip_file);
    void GetZipFileStatus(zip_t* zip, zip_stat_t* status, const std::string& file_name, uint32_t mode = 0);
}