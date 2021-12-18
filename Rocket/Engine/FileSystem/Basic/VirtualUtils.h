#pragma once
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/VirtualBlock.h"

namespace Rocket {
    std::string GetVirtualPath(const VirtualNodePtr& vnode);
    std::string GetRealPath(const VirtualNodePtr& vnode);
    //VirtualBlockPtr CreateBlockPath(VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    //VirtualNodePtr CreateNodePath(VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
    VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
    VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, const std::string& name);
}
