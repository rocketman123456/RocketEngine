#pragma once
#include "Basic/FileSystem.h"

namespace Rocket {
    enum class FileSystemType : int32_t {
        Native = 0,
        Memory,
        Compressed,
    };

    class FileSystemFactory {
    public:

    };
}
