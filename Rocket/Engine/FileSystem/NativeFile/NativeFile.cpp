#include "FileSystem/NativeFile/NativeFile.h"

namespace Rocket {
    NativeFile::NativeFile(const VirtualNodePtr& vnode_, const std::string& path_, const std::string& alias_path_)
        : vnode(vnode_), path(path_), alias_path(alias_path_) {}
}
