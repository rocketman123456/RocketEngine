#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <vector>
#include <functional>

namespace Rocket {
    // MaxPQ -> Compare = less<T>
    // MinPQ -> Compare = greater<T>
    template<typename T, typename Compare = std::less<T>>
    class PriorityQueue {
    public:
        explicit PriorityQueue(const Compare& x = Compare()) :compare(x) {}
        template<typename InputIt> PriorityQueue(InputIt first, InputIt last, const Compare& x = Compare())
            :pq(first, last), compare(x) {
            int N = pq.size();
            for (int k = (N-1) / 2; k >= 0; --k)
                sink(k);
        }

        void push(const T &t) {
            pq.push_back(t);
            int N = pq.size();
            swim(N - 1);
        }

        T pop() {
            T ret = pq[0];
            int N = pq.size();
            pq[0] = pq[N-1];
            pq.pop_back();
            if(!pq.empty()) sink(0);
            return ret;
        }
        
        T top() const { return pq[0]; }
        bool empty() { return pq.empty(); }
        int size() { return pq.size(); }

        auto begin() { return pq.begin(); }
        auto end() { return pq.end(); }
    private:
        void swim(int k) {
            auto key = pq[k];
            while (k > 0 && compare(pq[(k-1) >> 1], key)) {
                //exch((k-1) >> 1, k);
                //k = (k - 1) >> 1;
                pq[k] = std::move(pq[(k - 1) >> 1]);
                k = (k - 1) >> 1;
            }
            pq[k] = std::move(key);
        }

        void sink(int k) {
            auto key = pq[k];
            int N = pq.size();
            while ((k<<1) +1 < N) {
                int j = (k << 1) + 1;
                if (j < N-1 && compare(pq[j], pq[j+1])) ++j;
                if (!compare(key, pq[j])) break;
                //exch(k, j);
                pq[k] = std::move(pq[j]);
                k = j;
            }
            pq[k] = std::move(key);
        }

        void exch(int i, int j) {
            std::swap(pq[i], pq[j]);
        }
    private:
        std::vector<T> pq;
	    Compare compare;
    };
} // namespace Rocket
