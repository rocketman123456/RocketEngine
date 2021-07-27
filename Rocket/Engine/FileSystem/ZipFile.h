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
    
    using ZipFileInfo = unz_file_info64;
    using ZipContentsMap = std::unordered_map<std::string, ZipFileInfo>;

    class ZipFile : public OsFile {
    public:
        ZipFile() = default;
        virtual ~ZipFile() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual void Finalize() final;

        int32_t GetNumFiles() const;
        std::string GetFilename(int32_t i) const;	
        int32_t GetFileLen(int32_t i) const;
        bool ReadFile(int32_t i, void* buffer);
        int32_t Find(const std::string& path) const;

        /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
        virtual std::size_t Read(FileBuffer& buffer, std::size_t length) final;
        virtual std::size_t ReadAll(FileBuffer& buffer) final;
        /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
        virtual std::size_t Write(FileBuffer& buffer, std::size_t length) final;
        /// Seeks to the desired position
        virtual void Seek(std::size_t position) final;
        /// Seeks to the end of the file
        virtual void SeekToEnd(void) final;
        /// Skips a certain amount of bytes
        virtual void Skip(std::size_t bytes) final;
        /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
        virtual std::size_t Tell(void) const final;

        /// Asynchronously reads from the file into a buffer
        ZipAsyncFileOperation ReadAsync(FileBuffer& buffer, std::size_t length);
        /// Asynchronously writes from a buffer into the file
        ZipAsyncFileOperation WriteAsync(FileBuffer& buffer, std::size_t length);
        // Added to show multi-threaded decompression
        bool ReadLargeFile(int32_t i, void* buffer, std::function<void(int32_t, bool&)> call_back);

    private:
        ZipContentsMap content_;
    };
}
