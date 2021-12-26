#pragma once
#include "FileSystem/Basic/VirtualNode.h"
#include "FileSystem/Basic/VirtualBlock.h"

namespace Rocket {
    std::string GetVirtualPath(const VirtualNodePtr& vnode);
    std::string GetRealPath(const VirtualNodePtr& vnode);
    VirtualBlockPtr CreateVirtualBlock(VirtualBlockPtr& root, const std::string& path);
    VirtualNodePtr CreateVirtualNode(VirtualBlockPtr& root, const std::string& dirs);
    //VirtualBlockPtr CreateBlockPath(VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    //VirtualNodePtr CreateNodePath(VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    // VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
    // VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    VirtualBlockPtr FindVirtualBlock(const VirtualBlockPtr& root, const std::string& dir);
    // VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, int32_t level);
    // VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::vector<std::string>& dirs);
    VirtualBlockPtr FindDeepestExistVirtualBlock(const VirtualBlockPtr& root, const std::string& dir);
    // VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::vector<std::string>& dirs, const std::string& name);
    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& dirs, const std::string& name);
    VirtualNodePtr FindVirtualNode(const VirtualBlockPtr& root, const std::string& file_path);
}
