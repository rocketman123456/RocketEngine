#include "FileSystem/NativeFile/NativeFileSystem.h"
#include "Log/Log.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace Rocket {
    NativeFileSystem::NativeFileSystem(const std::string& basic_, const std::string& alias_)
        : basic_path(basic_), alias_path(alias_) {}
    
    void NativeFileSystem::Initialize() {
        RK_TRACE(File, "Current path: {}, {}", basic_path, alias_path);
        root = std::make_shared<VirtualBlock>();
        fs::path path = basic_path;
    }

    void NativeFileSystem::BuildUpVirtualBlocks(const std::string& name, VirtualBlockPtr& block) {
    }

    void NativeFileSystem::Finalize() {

    }

    bool NativeFileSystem::IsInitialized() const {

    }

    const FileList& NativeFileSystem::Files(const std::string& dir) const {

    }

    FilePtr NativeFileSystem::OpenFile(const std::string& file_path, int32_t mode) {

    }

    void NativeFileSystem::CloseFile(const FilePtr& file) {

    }

    bool NativeFileSystem::CreateFile(const std::string& file_path) {

    }

    bool NativeFileSystem::RemoveFile(const std::string& file_path) {

    }

    bool NativeFileSystem::CreateDir(const std::string& dir_path) {

    }

    bool NativeFileSystem::RemoveDir(const std::string& dir_path) {

    }

    bool NativeFileSystem::IsFileExists(const std::string& file_path) const {

    }

    bool NativeFileSystem::IsFile(const std::string& file_path) const {

    }

    bool NativeFileSystem::IsDir(const std::string& file_path) const {

    }

    bool NativeFileSystem::IsReadOnly() const {

    }
}
