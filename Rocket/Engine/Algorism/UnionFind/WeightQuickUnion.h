#pragma once
#include "Algorism/UnionFind/UnionFind.h"

namespace Rocket {
    class WeightQuickUnion : implements UnionFind {
    public:
        WeightQuickUnion(int32_t n);
        virtual ~WeightQuickUnion();
        virtual void Union(int32_t p, int32_t q) final;
        virtual int32_t Find(int32_t p) final;
    private:
        int32_t* id_ = nullptr;
        int32_t* sz_ = nullptr;
    };
}
