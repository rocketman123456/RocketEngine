#pragma once

#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <condition_variable>
#include <stdexcept>
#include <ctime>
#include <cstdlib>

namespace Rocket {
    class ThreadPool {
    public:
        explicit ThreadPool(std::size_t thread_count = std::thread::hardware_concurrency());
        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator = (const ThreadPool&) = delete;
        ThreadPool& operator = (ThreadPool&&) = delete;

        using work_item_t = std::function<void(void)>;

        void DoWork(work_item_t wi);

    private:
        using work_item_ptr_t = std::unique_ptr<work_item_t>;
        using work_queue_t = std::queue<work_item_ptr_t>;

        work_queue_t queue_;
        std::mutex queue_lock_;
        std::condition_variable condition_;

        using threads_t = std::vector<std::thread>;
        threads_t threads_;
    };
}
