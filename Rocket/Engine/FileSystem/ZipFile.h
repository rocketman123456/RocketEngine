#pragma once
#include "FileSystem/File.h"

#include <cstdint>
#include <string>
#include <functional>
#include <cstdlib>
#include <atomic>

namespace Rocket {
    class ZipAsyncFileOperation {
    public:
        ZipAsyncFileOperation(const FileHandle& file, size_t position);
        ZipAsyncFileOperation(const ZipAsyncFileOperation& other);
        ZipAsyncFileOperation& operator=(const ZipAsyncFileOperation& other);
        ~ZipAsyncFileOperation() = default;
        /// Returns whether or not the asynchronous operation has finished
        bool HasFinished() const;
        /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
        size_t WaitUntilFinished() const;
        /// Cancels the asynchronous operation
        void Cancel();
    private:
        FileHandle file_;
        std::atomic_int32_t overlapped_;
    };

    class ZipFile {
    public:
        ZipFile();
        virtual ~ZipFile();

        int32_t Initialize(const std::string& path, const std::string& file_name);
        void Finalize();

        int32_t GetNumFiles() const;
        std::string GetFilename(int32_t i) const;	
        int32_t GetFileLen(int32_t i) const;
        bool ReadFile(int32_t i, void* buffer);
        int32_t Find(const std::string& path) const;

        // Added to show multi-threaded decompression
        bool ReadLargeFile(int32_t i, void* buffer, std::function<void(int32_t, bool&)> call_back);

        /// Synchronously reads from the file into a buffer. Returns the number of bytes read, or 0 if the operation failed.
        std::size_t Read(FileBuffer& buffer, std::size_t length);
        /// Synchronously writes from a buffer into the file. Returns the number of bytes written, or 0 if the operation failed.
        std::size_t Write(FileBuffer& buffer, std::size_t length);
        /// Seeks to the desired position
        void Seek(std::size_t position);
        /// Seeks to the end of the file
        void SeekToEnd(void);
        /// Skips a certain amount of bytes
        void Skip(std::size_t bytes);
        /// Returns the current position in the file, or INVALID_SET_FILE_POINTER (0xFFFFFFFF) if the operation failed.
        std::size_t Tell(void) const;

        /// Asynchronously reads from the file into a buffer
        ZipAsyncFileOperation ReadAsync(FileBuffer& buffer, std::size_t length);
        /// Asynchronously writes from a buffer into the file
        ZipAsyncFileOperation WriteAsync(FileBuffer& buffer, std::size_t length);

    private:
        std::string file_path_;
        std::string file_name_;
        FileHandle  file_;
    };
}
