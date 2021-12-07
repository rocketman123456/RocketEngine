#include "Memory/Allocator.h"
#include <cassert>

namespace Rocket {
    Allocator::Allocator(const std::size_t totalSize) : total_size(totalSize) {}
    Allocator::~Allocator() { total_size = 0; }
}
