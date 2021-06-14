#include "FileSystem/OsFile.h"
#include "Debug/Log.h"

namespace Rocket {
    namespace FileSystem {
        OsSyncFile::OsSyncFile(std::unique_ptr<OsFileHandle> handle) {
            file_ = std::move(handle);
            buffer_.reset(new OsFileBuffer);
            buffer_->data = nullptr;
            buffer_->length = 0;
        }

        OsSyncFile::OsSyncFile(std::unique_ptr<OsFileHandle> handle, size_t length) {
            file_ = std::move(handle);
            buffer_.reset(new OsFileBuffer);
            buffer_->data = nullptr;
            buffer_->length = length;
        }

        OsSyncFile::OsSyncFile(OsSyncFile&& other) {
            file_ = std::move(other.file_);
            buffer_ = std::move(other.buffer_);
        }

        OsSyncFile::OsSyncFile(OsSyncFile& other) {
            file_ = std::move(other.file_);
            buffer_ = std::move(other.buffer_);
        }

        OsSyncFile::~OsSyncFile() {
            if(buffer_.get() != nullptr) {
                // release buffer
            }
        }

        OsAsyncFile::OsAsyncFile(std::unique_ptr<OsFileHandle> handle) {
            file_ = std::move(handle);
            buffer_.reset(new OsFileBuffer);
            buffer_->data = nullptr;
            buffer_->length = 0;
        }

        OsAsyncFile::OsAsyncFile(std::unique_ptr<OsFileHandle> handle, size_t length) {
            file_ = std::move(handle);
            buffer_.reset(new OsFileBuffer);
            buffer_->data = nullptr;
            buffer_->length = length;
        }

        OsAsyncFile::OsAsyncFile(OsAsyncFile&& other) {
            file_ = std::move(other.file_);
            buffer_ = std::move(other.buffer_);
        }

        OsAsyncFile::OsAsyncFile(OsAsyncFile& other) {
            file_ = std::move(other.file_);
            buffer_ = std::move(other.buffer_);
        }

        OsAsyncFile::~OsAsyncFile() {
            if(buffer_.get() != nullptr) {
                // release buffer
            }
        }

        OsFileAsyncOperation::OsFileAsyncOperation(const OsFileAsyncOperation& other)
            : file_(other.file_), ref_count_(other.ref_count_.load()) {
            ++ref_count_;
        }
        
        OsFileAsyncOperation& OsFileAsyncOperation::operator=(const OsFileAsyncOperation& other) {
            if (this != &other) {
                file_ = other.file_;
                ref_count_ = other.ref_count_.load();
                ++ref_count_;
            }
            return *this;
        }
        
        OsFileAsyncOperation::~OsFileAsyncOperation(void) {
            --ref_count_;
            if (ref_count_ == 0) {
                RK_CORE_TRACE("OsFile Async Operation End");
            }
        }
    }
}
