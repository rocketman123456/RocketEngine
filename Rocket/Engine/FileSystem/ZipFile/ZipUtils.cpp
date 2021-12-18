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

    void GetZipFileStatus(zip_t* zip, zip_stat_t* status, const std::string& file_name, uint32_t mode) {
        assert(zip != nullptr && status != nullptr);
        zip_stat_init(status);
        zip_stat(zip, file_name.c_str(), mode, status);
    }
}
