#pragma once
#include "Containers/Queue/UnboundedQueue.h"
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

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
#include <future>

namespace Rocket {
    class SimpleThreadPool {
    public:
        explicit SimpleThreadPool(std::size_t thread_count = std::thread::hardware_concurrency()) {
            if(!thread_count)
                throw std::invalid_argument("bad thread count! must be non-zero!");

            auto worker = [this]() {
                while(true) {
                    proc_t f;
                    if(!queue_.pop(f))
                        break;
                    f();
                }
            };

            threads_.reserve(thread_count);
            while(thread_count--)
                threads_.emplace_back(worker);
        }

        ~SimpleThreadPool() {
            queue_.unblock();
            for(auto& thread : threads_)
                thread.join();
        }

        template<typename F, typename... Args>
        void EnqueueWork(F&& f, Args&&... args) {
            queue_.push([p = std::forward<F>(f), t = std::make_tuple(std::forward<Args>(args)...)]() { std::apply(p, t); });
        }

        template<typename F, typename... Args>
        [[nodiscard]] auto EnqueueTask(F&& f, Args&&... args) 
                -> std::future<std::invoke_result_t<F, Args...>> {
            using task_return_type = std::invoke_result_t<F, Args...>;
            using task_type = std::packaged_task<task_return_type()>;

            auto task = std::make_shared<task_type>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
            auto result = task->get_future();

            queue_.push([=]() { (*task)(); });

            return result;
        }

    private:
        using proc_t = std::function<void(void)>;
        using queue_t = UnboundedQueue<proc_t>;
        queue_t queue_;

        using threads_t = std::vector<std::thread>;
        threads_t threads_;
    };

    class ThreadPool {
    public:
        explicit ThreadPool(std::size_t thread_count = std::thread::hardware_concurrency()) 
            : queues_(thread_count), count_(thread_count) {
            if(!thread_count)
                throw std::invalid_argument("bad thread count! must be non-zero!");

            auto worker = [this](auto i) {
                while(true) {
                    proc_t f;
                    for(auto n = 0; n < count_ * K; ++n)
                        if(queues_[(i + n) % count_].try_pop(f))
                            break;
                    if(!f && !queues_[i].pop(f))
                        break;
                    f();
                }
            };

            threads_.reserve(thread_count);
            for(auto i = 0; i < thread_count; ++i)
                threads_.emplace_back(worker, i);
        }

        ~ThreadPool() {
            for(auto& queue : queues_)
                queue.unblock();
            for(auto& thread : threads_)
                thread.join();
        }

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator = (const ThreadPool&) = delete;
        ThreadPool& operator = (ThreadPool&&) = delete;

        template<typename F, typename... Args>
        void EnqueueWork(F&& f, Args&&... args) {
            auto work = [p = std::forward<F>(f), t = std::make_tuple(std::forward<Args>(args)...)]() { std::apply(p, t); };
            auto i = index_++;

            for(auto n = 0; n < count_ * K; ++n)
                if(queues_[(i + n) % count_].try_push(work))
                    return;

            queues_[i % count_].push(std::move(work));
        }

        template<typename F, typename... Args>
        [[nodiscard]] auto EnqueueTask(F&& f, Args&&... args) 
                -> std::future<std::invoke_result_t<F, Args...>> {
            using task_return_type = std::invoke_result_t<F, Args...>;
            using task_type = std::packaged_task<task_return_type()>;

            auto task = std::make_shared<task_type>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
            auto work = [=]() { (*task)(); };
            auto result = task->get_future();
            auto i = index_++;

            for(auto n = 0; n < count_ * K; ++n)
                if(queues_[(i + n) % count_].try_push(work))
                    return result;

            queues_[i % count_].push(std::move(work));

            return result;
        }

    private:
        using proc_t = std::function<void(void)>;
        using queue_t = UnboundedQueue<proc_t>;
        using queues_t = std::vector<queue_t>;
        queues_t queues_;

        using threads_t = std::vector<std::thread>;
        threads_t threads_;

        const std::size_t count_;
        std::atomic_uint index_ = 0;

        inline static const unsigned int K = 2;
    };
}
