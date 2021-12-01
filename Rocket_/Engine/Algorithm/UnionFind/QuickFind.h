#pragma once
#include "Algorithm/UnionFind/UnionFind.h"

#include <cassert>
#include <cstdint>

namespace Rocket {
    class QuickFind : _implements_ UnionFind {
    public:
        QuickFind(int32_t n);
        virtual ~QuickFind();
        virtual void Union(int32_t p, int32_t q) final;
        virtual int32_t Find(int32_t p) final;
    private:
        int32_t* id_ = nullptr;
    };

    QuickFind::QuickFind(int32_t n) {
        size_ = n;
        id_ = new int32_t[n];
        for(int i = 0; i < n; ++i) {
            id_[i] = i;
        }
    }

    QuickFind::~QuickFind() {
        if(id_) delete id_;
    }

    void QuickFind::Union(int32_t p, int32_t q) {
        auto pId = Find(p);
        auto qId = Find(q);

        if(pId == qId) return;

        for(int i = 0; i < size_; ++i) {
            if(id_[i] == pId) {
                id_[i] = qId;
            }
        }
        count_--;
    }

    int32_t QuickFind::Find(int32_t p) {
        assert(p < size_ && p >= 0);
        return id_[p];
    }
}