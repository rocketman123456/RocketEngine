#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>

namespace Rocket {
    class Semphore {
    public:
        explicit Semphore(unsigned int count = 0)
            : count_(count) {}

        void post() {
            {
                std::unique_lock lock(mutex_);
                ++count_;
            }
            cv_.notify_one();
        }

        void post(unsigned int count) {
            {
                std::unique_lock lock(mutex_);
                count_ += count;
            }
            cv_.notify_all();
        }

        void wait() {
            std::unique_lock lock(mutex_);
            cv_.wait(lock, [this]() { return count_ != 0; });
            --count_;
        }

        template<typename Rep, typename Period>
        bool wait_for(const std::chrono::duration<Rep, Period>& t) {
            std::unique_lock lock(mutex_);
            if(!cv_.wait_for(lock, t, [this]() { return count_ != 0; }))
                return false;
            --count_;
            return true;
        }

        template<typename Clock, typename Duration>
        bool wait_until(const std::chrono::time_point<Clock, Duration>& t) {
            std::unique_lock lock(mutex_);
            if(!cv_.wait_until(lock, t, [this]() { return count_ != 0; }))
                return false;
            --count_;
            return true;
        }

    private:
        unsigned int count_;
        std::mutex mutex_;
        std::condition_variable cv_;
    };
}
