#include "FileSystem/OsFileAsync.h"

namespace Rocket {
    OsAsyncFileOperation::OsAsyncFileOperation(const OsAsyncFileOperation& other) : file_(other.file_) {
        int32_t temp = other.overlapped_;
        overlapped_ = temp;
        overlapped_++; 
    }

    OsAsyncFileOperation& OsAsyncFileOperation::operator=(const OsAsyncFileOperation& other) {
        if (this != &other) {
            file_ = other.file_;
            int32_t temp = other.overlapped_;
            overlapped_ = temp;
            overlapped_++;
        }
        return *this;
    }

    int32_t OsFileAsync::Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) {
        return 0;
    }
    
    void OsFileAsync::Finalize() {
        
    }
}