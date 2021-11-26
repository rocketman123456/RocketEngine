#pragma once

#include <mutex>
#include <thread>

namespace Rocket {
    class ReadWriteLock {
    public:
        ReadWriteLock() = default;
        ~ReadWriteLock() = default;

        void ReadLock() {
            read_.lock();
            if(++read_count_ == 1) {
                write_.lock();
            }
            read_.unlock();
        }

        void ReadUnlock() {
            read_.lock();
            if(--read_count_ == 0) {
                write_.unlock();
            }
            read_.unlock();
        }

        inline void WriteLock() { write_.lock(); }
        inline void WriteUnlock() { write_.unlock(); }
    private:
        std::mutex read_;
        std::mutex write_;
        int read_count_ = 0;
    };
}
