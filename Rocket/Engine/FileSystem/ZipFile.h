#pragma once
#include "FileSystem/OsFile.h"

#include <zlib.h>
#include <zip.h>
#include <unzip.h>

#include <cstdint>
#include <string>
#include <functional>
#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <future>

namespace Rocket {
    struct ZipFileInfo {
        unz64_file_pos_s file_pos;
        unz_file_info64 file_info;
    };

    using UnzipGlobalInfo = unz_global_info64;
    //using ZipGlobalInfo = zip_global_info64;
    using ZipContentsMap = std::unordered_map<std::string, ZipFileInfo>;

    class ZipFile : public OsFile {
    // public:
    //     ALIGNED_OPERATOR_NEW;
    public:
        ZipFile() = default;
        virtual ~ZipFile() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual int32_t Initialize(const std::string& path, FileOperateMode mode) final;
        virtual void Finalize() final;

        int64_t GetNumFiles() const { return unzip_info_.number_entry; }
        ZipFileInfo Find(const std::string& path) const;

        /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
        std::size_t ReadFile(FileBuffer& buffer, const std::string& path);
        /// Synchronously write from the buffer into a file. Returns the number of bytes write, or 0 if the operation failed.
        std::size_t WriteFile(FileBuffer& buffer, const std::string& path);

        /// Asynchronously reads from the file into a buffer
        std::future<std::size_t> ReadAsync(FileBuffer& buffer, const std::string& path);
        std::future<std::size_t> ReadAllAsync(std::vector<FileBuffer>& buffers);
        /// Asynchronously writes from a buffer into the file
        std::future<std::size_t> WriteAsync(FileBuffer& buffer, const std::string& path);
        std::future<std::size_t> WriteAllAsync(std::vector<FileBuffer>& buffers, std::vector<std::string> paths);

    private:
        int32_t UnzipInit();
        int32_t ZipInit();

        UnzipGlobalInfo  unzip_info_;
        ZipContentsMap content_;

        std::recursive_mutex write_mutex;
    };

    using ZipFilePtr = std::unique_ptr<ZipFile>;
}
