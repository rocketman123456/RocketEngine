#pragma once
#include "Algorism/UnionFind/UnionFind.h"

namespace Rocket {
    class QuickUnion : _implements_ UnionFind {
    public:
        QuickUnion(int32_t n);
        virtual ~QuickUnion();
        virtual void Union(int32_t p, int32_t q) final;
        virtual int32_t Find(int32_t p) final;
    private:
        int32_t* id_ = nullptr;
    };
}
