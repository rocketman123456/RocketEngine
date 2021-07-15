#pragma once

// FIFO
namespace Rocket {
    template<typename T>
    class Queue {
    public:
        Queue();

        void Enqueue(const T& item);
        T Dequeue();

        bool IsEmpty();
        int32_t Size();
    };
}
