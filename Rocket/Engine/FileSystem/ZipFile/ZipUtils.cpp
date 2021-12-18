#include "FileSystem/ZipFile/ZipUtils.h"
#include "Log/Log.h"

#include <cassert>

namespace Rocket {
    zip_file_t* OpenZipFile(zip_t* zip, const std::string& file_name, uint32_t mode) {
        assert(zip != nullptr);
        auto file = zip_fopen(zip, file_name.c_str(), mode);
        if(file == nullptr) {
            RK_WARN(File, "Open File Not Exist in Zip: {}", file_name);
        }
        return file;
    }

    void CloseZipFile(zip_file_t* zip_file) {
        zip_fclose(zip_file);
    }

    void GetZipFileStatus(zip_t* zip, zip_stat_t* status, const std::string& file_name, uint32_t mode) {
        assert(zip != nullptr && status != nullptr);
        zip_stat_init(status);
        auto result = zip_stat(zip, file_name.c_str(), mode, status);
        if(result < 0) {
            RK_WARN(File, "Get File Info in Zip Error: {}", file_name);
        }
    }
}
