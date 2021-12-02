#pragma once
#include "Algorithm/UnionFind/UnionFind.h"

#include <cassert>

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

    QuickUnion::QuickUnion(int32_t n) {
        size_ = n;
        id_ = new int32_t[n];
        for(int i = 0; i < n; ++i) {
            id_[i] = i;
        }
    }

    QuickUnion::~QuickUnion() {
        if(id_) delete id_;
    }

    void QuickUnion::Union(int32_t p, int32_t q) {
        auto p_root = Find(p);
        auto q_root = Find(q);

        if(p_root == q_root) return;

        id_[p_root] = q_root;
        count_--;
    }
    
    int32_t QuickUnion::Find(int32_t p) {
        assert(p < size_ && p >= 0);
        while(p != id_[p]) p = id_[p];
        return p;
    }
}