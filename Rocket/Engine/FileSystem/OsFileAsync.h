#pragma once
#include "FileSystem/OsFile.h"

namespace Rocket {
    // TODO : finish async file operation
    class OsAsyncFileOperation : implements FileOperation {
    public:
        OsAsyncFileOperation(const FileHandle& file, size_t position);
        OsAsyncFileOperation(const OsAsyncFileOperation& other);
        OsAsyncFileOperation& operator=(const OsAsyncFileOperation& other);
        ~OsAsyncFileOperation() = default;
        /// Returns whether or not the asynchronous operation has finished
        bool HasFinished() const final { return false; }
        /// Waits until the asynchronous operation has finished. Returns the number of transferred bytes.
        size_t WaitUntilFinished() const final { return 0; }
        /// Cancels the asynchronous operation
        void Cancel() final {}
    };

    class OsFileAsync : public OsFile {
    public:
        OsFileAsync() = default;
        virtual ~OsFileAsync() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual void Finalize() final;
        ////////////////////////////////////////////////////////////////////////
        // Asynchronous API
        ////////////////////////////////////////////////////////////////////////

        /// Asynchronously reads from the file into a buffer
        OsAsyncFileOperation ReadAsync(FileBuffer& buffer, std::size_t length);
        /// Asynchronously writes from a buffer into the file
        OsAsyncFileOperation WriteAsync(FileBuffer& buffer, std::size_t length);
    protected:
        bool async_;
    };

    using OsFileAsyncPtr = std::unique_ptr<OsFileAsync>;
}
