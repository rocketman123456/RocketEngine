#pragma once
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/VirtualBlock.h"

namespace Rocket {
    std::string GetVirtualPath(const VirtualNodePtr& vnode);
    std::string GetRealPath(const VirtualNodePtr& vnode);
    VirtualBlockPtr CreateVirtualBlock(VirtualBlockPtr& root, const std::string& path);
    VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::string& dir);
    VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::string& dir);
    VirtualNodePtr CreateVirtualNode(VirtualBlockPtr& root, const std::string& dirs);
    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& dirs, const std::string& name);
    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& file_path);
}
