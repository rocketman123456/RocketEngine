#pragma once

#include <mutex>
#include <queue>
#include <list>
#include <utility>
#include <stdexcept>
#include <condition_variable>

namespace Rocket {
    template <typename T>
    class UnboundedQueue {
    public:
        explicit UnboundedQueue(bool block = true) : m_block{block} {}

        void push(const T& item) {
            {
                std::scoped_lock guard(m_queue_lock);
                m_queue.push_back(item);
            }
            m_condition.notify_one();
        }

        void push(T &&item) {
            {
                std::scoped_lock guard(m_queue_lock);
                m_queue.push_back(std::move(item));
            }
            m_condition.notify_one();
        }

        template <typename... Args>
        void emplace(Args &&...args) {
            {
                std::scoped_lock guard(m_queue_lock);
                m_queue.emplace_back(std::forward<Args>(args)...);
            }
            m_condition.notify_one();
        }

        bool try_push(const T &item) {
            {
                std::unique_lock lock(m_queue_lock, std::try_to_lock);
                if (!lock)
                    return false;
                m_queue.push_back(item);
            }
            m_condition.notify_one();
            return true;
        }

        bool try_push(T &&item) {
            {
                std::unique_lock lock(m_queue_lock, std::try_to_lock);
                if (!lock)
                    return false;
                m_queue.push_back(std::move(item));
            }
            m_condition.notify_one();
            return true;
        }

        bool pop(T &item) {
            std::unique_lock guard(m_queue_lock);
            m_condition.wait(guard, [&](){ return !m_queue.empty() || !m_block; });
            if (m_queue.empty())
                return false;
            item = std::move(m_queue.front());
            m_queue.pop_front();
            return true;
        }

        bool try_pop(T &item) {
            std::unique_lock lock(m_queue_lock, std::try_to_lock);
            if (!lock || m_queue.empty())
                return false;
            item = std::move(m_queue.front());
            m_queue.pop_front();
            return true;
        }

        std::size_t size() const {
            std::scoped_lock guard(m_queue_lock);
            return m_queue.size();
        }

        bool empty() const {
            std::scoped_lock guard(m_queue_lock);
            return m_queue.empty();
        }

        void block() {
            std::scoped_lock guard(m_queue_lock);
            m_block = true;
        }

        void unblock() {
            {
                std::scoped_lock guard(m_queue_lock);
                m_block = false;
            }
            m_condition.notify_all();
        }

        bool blocking() const {
            std::scoped_lock guard(m_queue_lock);
            return m_block;
        }

        // Not Thread Safe
        auto begin() { m_queue.begin(); }
        auto end() { m_queue.end(); }

    private:
        using queue_t = std::list<T>;
        queue_t m_queue;
        bool m_block;
        mutable std::mutex m_queue_lock;
        std::condition_variable m_condition;
    };
}
