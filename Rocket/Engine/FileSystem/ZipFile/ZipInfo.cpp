#include "FileSystem/ZipFile/ZipInfo.h"
#include "Memory/MemoryDefine.h"
#include "Utils/StringUtils.h"
#include "Log/Log.h"

#include <miniz.h>
#include <miniz_zip.h>
#include <cstring>      // memset
#include <sys/stat.h>

namespace Rocket {
    ZipInfo::ZipInfo(const std::string& zip_path) : file_name(zip_path) {
        zip_archive = static_cast<mz_zip_archive*>(new mz_zip_archive{});
        std::memset(zip_archive, 0, sizeof(mz_zip_archive));
        mz_bool status = mz_zip_reader_init_file((mz_zip_archive*)zip_archive, zip_path.c_str(), 0);
        if(!status) {
            RK_ERROR(File, "Cannot open zip file: {}", zip_path);
            assert("Cannot open zip file" && false);
        }

        for (mz_uint i = 0; i < mz_zip_reader_get_num_files((mz_zip_archive*)zip_archive); i++) {
            mz_zip_archive_file_stat file_stat;
            if (!mz_zip_reader_file_stat((mz_zip_archive*)zip_archive, i, &file_stat)) {
                RK_ERROR(File, "Cannot read entry {} with index: {}} from zip archive", zip_path, i);
                continue;
            }
            entries[file_stat.m_filename] = std::make_tuple(file_stat.m_file_index, file_stat.m_uncomp_size);
        }
    }

    ZipInfo::~ZipInfo() {
        delete zip_archive;
    }

    bool ZipInfo::MapFile(const std::string& filename, std::vector<uint8_t>& data) {
        EntryMap::const_iterator it = entries.find(filename);
        if (it == entries.end()) {
            return false;
        }
        uint32_t index = std::get<0>(it->second);
        uint64_t size = std::get<1>(it->second);
        data.resize((size_t)size);
        bool ok = mz_zip_reader_extract_to_mem_no_alloc(
            (mz_zip_archive*)zip_archive, index, data.data(), std::size_t(size), 0, 0, 0);
        return ok;
    }

    bool ZipInfo::IsReadOnly() const {
        struct stat file_stat;
        if (stat(FileName().c_str(), &file_stat) < 0) {
            return false;
        }
#ifdef RK_WINDOWS
        return (file_stat.st_mode & _S_IWRITE);
#else
        return (file_stat.st_mode & S_IWUSR);
#endif
    }
}