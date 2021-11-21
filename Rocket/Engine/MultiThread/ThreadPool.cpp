#include "MultiThread/ThreadPool.h"

namespace Rocket {
    ThreadPool::ThreadPool(std::size_t thread_count) {
        if(!thread_count)
            throw std::invalid_argument("bad thread count! must be non-zero!");

        threads_.reserve(thread_count);

        for(auto i = 0; i < thread_count; ++i) {
            threads_.push_back(std::thread([this]() {
                while(true) {
                    work_item_ptr_t work{nullptr};
                    {
                        std::unique_lock guard(queue_lock_);
                        condition_.wait(guard, [&]() { return !queue_.empty(); });
                        work = std::move(queue_.front());
                        queue_.pop();
                    }
                    if(!work) {
                        break;
                    }
                    (*work)();
                }
            }));
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::unique_lock guard(queue_lock_);
            for(auto& t : threads_)
                queue_.push(work_item_ptr_t{nullptr});
        }
        for(auto& t : threads_)
            t.join();
    }

    void ThreadPool::DoWork(work_item_t wi)
    {
        auto work_item = std::make_unique<work_item_t>(std::move(wi));
        {
            std::unique_lock guard(queue_lock_);
            queue_.push(std::move(work_item));
        }
        condition_.notify_one();
    }
}
