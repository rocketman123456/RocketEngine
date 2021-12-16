#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/File.h"

namespace Rocket {
    class MemoryFile : _implements_ File {
    public:
        // For File System
        explicit MemoryFile(const VirtualNodePtr& vnode_);
        explicit MemoryFile(const std::string& path_, const std::string& virtual_path_);
        explicit MemoryFile(const std::string& path_);
        virtual ~MemoryFile() = default;

        inline VirtualNodePtr VNode() const final { return vnode; }
        inline std::string VirtualPath() const final { return virtual_path; }
        inline std::string RealPath() const final { return real_path; }
        inline std::size_t Size() const final { return file_data.size(); }
        inline bool IsOpened() const final { return is_opened; }
        inline bool IsReadOnly() const final { return is_read_only; }

        // Should Check File Status Before These Operation
        // these functions don't promise correct behavior 
        // if you give a wrong file name input
        void Open(int32_t mode) final;
        void Close() final;
        std::size_t Seek(std::size_t offset, FileEnum::Origin origin) final;
        std::size_t Tell() final;
        std::size_t Read(FileBuffer& data) final;
        std::size_t Write(const FileBuffer& data) final;
    private:
        VirtualNodePtr vnode = nullptr;
        std::string virtual_path = "";
        std::string real_path = "";
        FileBuffer file_data;
        std::size_t seek_pos = 0;
        int32_t mode = 0;
        bool is_read_only = false;
        bool is_opened = false;
    };

    CLASS_PTR(MemoryFile);
}
