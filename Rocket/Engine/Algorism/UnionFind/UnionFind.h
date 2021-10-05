#pragma once
#include "Core/Declare.h"

#include <cstdint>

namespace Rocket {
    _Interface_ UnionFind {
    public:
        virtual ~UnionFind();
        virtual void Union(int32_t p, int32_t q) = 0;
        virtual int32_t Find(int32_t p) = 0;
        bool Connected(int32_t p, int32_t q) { return Find(p) == Find(q); }
        inline int32_t Count() { return count_; }
    protected:
        int32_t count_ = 0;
        int32_t size_ = 0;
    };
}