#pragma once
#include "Core/MemoryDefine.h"
#include "FileSystem/Basic/File.h"
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/VirtualBlock.h"
#include "Utils/CRC.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <gsl/gsl>

namespace Rocket {
    using VNodeList = std::vector<VirtualNodePtr>;
    using VBlockList = std::vector<VirtualBlockPtr>;
    using VNodeMap = std::unordered_map<std::string, VirtualNodePtr>;
    using VBlockMap = std::unordered_map<std::string, VirtualBlockPtr>;

    // TODO : add unimplement functions 
    // and merge some common code into base class
    // most operations of the file system share same logic
    _Interface_ FileSystem {
    public:
        FileSystem(const std::string& real_path, const std::string& virtual_path);
        virtual ~FileSystem() = default;
        // Basic Operation
        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        inline virtual bool IsInitialized() const { return is_initialized; }
        // For File System
        virtual void SetVirtualPath(const std::string& basic) = 0;
        virtual void SetRealPath(const std::string& alias) = 0;
        [[nodiscard]] inline const std::string& VirtualPath() const { return virtual_path; }
        [[nodiscard]] inline const std::string& RealPath() const { return real_path; }
        [[nodiscard]] inline VirtualBlockPtr RootBlock() const { return root; }
        [[nodiscard]] inline const VNodeMap& NodeMap() const { return node_map; }
        [[nodiscard]] inline const VBlockMap& BlockMap() const { return block_map; }
        [[nodiscard]] VNodeList VNodes(const std::string& dir) const;
        [[nodiscard]] VBlockList VBlocks(const std::string& dir) const;
        // Basic Judgement
        [[nodiscard]] bool IsFileExists(const std::string& file_path) const;
        [[nodiscard]] bool IsDirExists(const std::string& dir_path) const;
        [[nodiscard]] bool IsFile(const std::string& file_path) const;
        [[nodiscard]] bool IsDir(const std::string& file_path) const;
        [[nodiscard]] virtual bool IsReadOnly() const = 0;
        // File Operation
        [[nodiscard]] virtual FilePtr GetFilePointer(const std::string& file_path) = 0;
        void OpenFile(const FilePtr& file, int32_t mode);
        void CloseFile(const FilePtr& file);
        [[nodiscard]] std::size_t ReadFile(const FilePtr& file, FileBuffer* data);
        [[nodiscard]] std::size_t WriteFile(FilePtr& file, const FileBuffer& data);
        
        [[nodiscard]] virtual bool CreateFile(const std::string& file_path);
        [[nodiscard]] virtual bool RemoveFile(const std::string& file_path);
        [[nodiscard]] virtual bool MoveFile(const std::string& src, const std::string& dst);
        [[nodiscard]] virtual bool RenameFile(const std::string& src, const std::string& dst);
        [[nodiscard]] virtual bool CopyFile(const std::string& src, const std::string& dst);
        [[nodiscard]] virtual std::size_t FileSize(const FilePtr& file) const;
        // Dir Operation
        [[nodiscard]] virtual bool CreateDir(const std::string& dir_path);
        [[nodiscard]] virtual bool RemoveDir(const std::string& dir_path);
        [[nodiscard]] virtual bool MoveDir(const std::string& src, const std::string& dst);
        [[nodiscard]] virtual bool RenameDir(const std::string& src, const std::string& dst);
        [[nodiscard]] virtual bool CopyDir(const std::string& src, const std::string& dst);

    protected:
        VirtualBlockPtr root = nullptr;
        std::string real_path = "";
        std::string virtual_path = "";
        VNodeMap node_map = {};
        VBlockMap block_map = {};
        bool is_initialized = false;
    };

    CLASS_PTR(FileSystem);
}
