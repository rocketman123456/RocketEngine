#pragma once
#include "Core/Core.h"

#include <string>
#include <list>
#include <map>
#include <memory>

namespace Rocket {
    namespace FileSystem {
        //typedef std::list<std::shared_ptr<ResHandle>> ResHandleList;					// lru list
        //typedef std::map<std::string, std::shared_ptr<ResHandle>> ResHandleMap;		// maps indentifiers to resource data

        using ResourceLoaders = std::list<std::shared_ptr<IResourceLoader>>;

        Interface IResource {
        public:
            IResource(const std::string& name) : name_(name) {}
            virtual ~IResource() = default;
            virtual std::string ToString() = 0;
            std::string name_;
        };

        Interface IResourceData {
        public:
            virtual std::string ToString() = 0;
        };

        Interface IResourceFile {
        };

        Interface IResourceLoader {
        };
    }
}
