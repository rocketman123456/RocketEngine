#pragma once
#include "FileSystem/OsFile.h"

#include <cstdint>
#include <string>
#include <functional>
#include <cstdlib>
#include <atomic>
#include <unordered_map>

#include <zlib.h>
#include <zip.h>
#include <unzip.h>

namespace Rocket {
    // TODO : finish async file operation
    class ZipAsyncFileOperation : implements FileOperation {
    public:
        ZipAsyncFileOperation(const FileHandle& file, size_t position);
        ZipAsyncFileOperation(const ZipAsyncFileOperation& other);
        ZipAsyncFileOperation& operator=(const ZipAsyncFileOperation& other);
        ~ZipAsyncFileOperation() = default;
        /// Returns whether or not the asynchronous operation has finished
        bool HasFinished() const final { return false; }
        /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
        size_t WaitUntilFinished() const final { return 0; }
        /// Cancels the asynchronous operation
        void Cancel() final {}
    };

    struct ZipFileInfo {
        unz64_file_pos_s file_pos;
        unz_file_info64 file_info;
    };
    
    using ZipGlobalInfo = unz_global_info64;
    using ZipContentsMap = std::unordered_map<std::string, ZipFileInfo>;

    class ZipFile : public OsFile {
    public:
        ZipFile() = default;
        virtual ~ZipFile() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual void Finalize() final;

        int64_t GetNumFiles() const { return zip_info_.number_entry; }
        ZipFileInfo Find(const std::string& path) const;

        /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
        std::size_t ReadFile(FileBuffer& buffer, const std::string& path);
        /// Synchronously write from the buffer into a file. Returns the number of bytes write, or 0 if the operation failed.
        std::size_t WriteFile(FileBuffer& buffer, const std::string& path);

        /// Asynchronously reads from the file into a buffer
        ZipAsyncFileOperation ReadAsync(FileBuffer& buffer, std::size_t length);
        /// Asynchronously writes from a buffer into the file
        ZipAsyncFileOperation WriteAsync(FileBuffer& buffer, std::size_t length);
        // Added to show multi-threaded decompression
        bool ReadLargeFile(int32_t i, void* buffer, std::function<void(int32_t, bool&)> call_back);

    private:
        int32_t UnzipInit();
        int32_t ZipInit();

        ZipGlobalInfo  zip_info_;
        ZipContentsMap content_;
    };

    using ZipFilePtr = std::unique_ptr<ZipFile>;
}
