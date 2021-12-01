#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"
#include "FileSystem/FileBasic.h"

#include <string>

namespace Rocket {
    // TODO : Add Cache For Loaded Files
    _Interface_ OsFile {
    public:
        virtual int32_t Initialize(const std::string& path, const std::string& file_name, FileOperateMode mode) = 0;
        virtual int32_t Initialize(const std::string& path, FileOperateMode mode) = 0;
        virtual void Finalize() = 0;

        inline FileOperateMode GetFileOperateMode() const { return mode_; }
        inline const FileHandle* GetFileHandle() const { return &file_; }
        
    protected:
        FileOperateMode mode_;
        FileHandle file_;
        bool initialized_;
    };
}