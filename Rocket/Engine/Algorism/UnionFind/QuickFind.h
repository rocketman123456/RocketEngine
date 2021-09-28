#pragma once
#include "Algorism/UnionFind/UnionFind.h"

#include <cstdint>

namespace Rocket {
    class QuickFind : implements UnionFind {
    public:
        QuickFind(int32_t n);
        virtual ~QuickFind();
        virtual void Union(int32_t p, int32_t q) final;
        virtual int32_t Find(int32_t p) final;
    private:
        int32_t* id_ = nullptr;
    };
}