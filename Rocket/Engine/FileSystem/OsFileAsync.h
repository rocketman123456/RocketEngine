#pragma once
#include "FileSystem/OsFileSync.h"

#include <memory>
#include <future>

namespace Rocket {
    class OsFileAsync : _implements_ OsFileSync {
    public:
        OsFileAsync() = default;
        virtual ~OsFileAsync() = default;

        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) final;
        virtual void Finalize() final;
        ////////////////////////////////////////////////////////////////////////
        // Asynchronous API
        ////////////////////////////////////////////////////////////////////////

        /// Asynchronously reads from the file into a buffer
        std::future<std::size_t> ReadAsync(FileBuffer& buffer, std::size_t length);
        std::future<std::size_t> ReadAllAsync(FileBuffer& buffer);
        /// Asynchronously writes from a buffer into the file
        std::future<std::size_t> WriteAsync(FileBuffer& buffer, std::size_t length);
        std::future<std::size_t> WriteAllAsync(FileBuffer& buffer);
    protected:
        bool async_;
    };

    using OsFileAsyncPtr = std::unique_ptr<OsFileAsync>;
}
