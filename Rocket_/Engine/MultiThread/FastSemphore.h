#pragma once
#include "MultiThread/Semphore.h"

namespace Rocket {
    class FastSemphore {
    public:
        explicit FastSemphore(unsigned int count = 0)
        : count_(count), semaphore_(0) {}

        void post()
        {
            int count = count_.fetch_add(1, std::memory_order_release);
            if (count < 0)
                semaphore_.post();
        }

        void wait()
        {
            int count = count_.fetch_sub(1, std::memory_order_acquire);
            if (count < 1)
                semaphore_.wait();
        }

    private:
        std::atomic_int count_;
        Semaphore semaphore_;
    };
}
