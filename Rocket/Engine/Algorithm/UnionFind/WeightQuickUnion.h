#pragma once
#include "Algorithm/UnionFind/UnionFind.h"

#include <cassert>

namespace Rocket {
    class WeightQuickUnion : _implements_ UnionFind {
    public:
        WeightQuickUnion(int32_t n);
        virtual ~WeightQuickUnion();
        virtual void Union(int32_t p, int32_t q) final;
        virtual int32_t Find(int32_t p) final;
    private:
        int32_t* id_ = nullptr;
        int32_t* sz_ = nullptr;
    };

    WeightQuickUnion::WeightQuickUnion(int32_t n) {
        size_ = n;
        id_ = new int32_t[n];
        for(int i = 0; i < n; ++i) {
            id_[i] = i;
        }
        sz_ = new int32_t[n];
        for(int i = 0; i < n; ++i) {
            sz_[i] = 1;
        }
    }

    WeightQuickUnion::~WeightQuickUnion() {
        if(id_) delete id_;
        if(sz_) delete sz_;
    }

    void WeightQuickUnion::Union(int32_t p, int32_t q) {
        auto p_root = Find(p);
        auto q_root = Find(q);

        if(p_root == q_root) return;

        if(sz_[p_root] < sz_[q_root]) {
            id_[p_root] = q_root;
            sz_[q_root] += sz_[p_root];
        }
        else {
            id_[q_root] = p_root;
            sz_[q_root] += sz_[p_root];
        }
        count_--;
    }
    
    int32_t WeightQuickUnion::Find(int32_t p) {
        assert(p < size_ && p >= 0);
        while(p != id_[p]) p = id_[p];
        return p;
    }
}
