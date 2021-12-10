#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/VirtualNode.h"

#include <fstream>

namespace Rocket {
    class NativeFile : _implements_ File {
    public:
        NativeFile(const VirtualNodePtr& vnode_, const std::string& path_, const std::string& alias_path_);
        virtual ~NativeFile() = default;

        std::size_t Size() const final;
        bool IsReadOnly() const final;
        bool IsOpened() const final;
        void Open(int32_t mode) final;
        void Close() final;
        std::size_t Seek(std::size_t offset, int32_t origin) final;
        std::size_t Tell() final;
        std::size_t Read(gsl::span<gsl::byte>& data) final;
        std::size_t Write(gsl::span<gsl::byte> data) final;
    public:
        VirtualNodePtr vnode = nullptr;
        std::string path;
        std::string alias_path;
    };
}
