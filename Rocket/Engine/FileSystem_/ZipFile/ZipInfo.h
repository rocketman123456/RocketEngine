#pragma once
#include "Core/MemoryDefine.h"

#include <miniz.h>
#include <miniz_zip.h>
#include <gsl/gsl>

#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>

namespace Rocket {
    // TO Pre-Store Zip File Infos, make a lookup table
    class ZipInfo {
    public:
        ZipInfo(const std::string& zip_path);
        ~ZipInfo();

        bool MapFile(const std::string& filename, std::vector<uint8_t>& data);
        bool IsReadOnly() const;

        inline const std::string& FileName() const { return file_name; }
    private:
        using EntryMap = std::unordered_map<std::string, std::tuple<uint32_t, uint64_t>>;
        using ZipArchive = mz_zip_archive;
        std::string file_name;
        gsl::owner<ZipArchive*> zip_archive;
        EntryMap entries;
    };

    CLASS_PTR(ZipInfo);
}
