#pragma once
#include "Memory/Allocator.h"

namespace Rocket {
    template <class T>
    class StackLinkedList {
    public:
        struct Node {
            T data;
            Node* next = nullptr;
        };
        Node* head = nullptr;
    public:
        StackLinkedList() = default;
        StackLinkedList(StackLinkedList &stackLinkedList) = delete;
        void push(Node* newNode);
        Node* pop();
    };

    template <class T>
    void StackLinkedList<T>::push(Node* newNode) {
        newNode->next = head;
        head = newNode;
    }

    template <class T>
    typename StackLinkedList<T>::Node* StackLinkedList<T>::pop() {
        Node* top = head;
        head = head->next;
        return top;
    }

    class PoolAllocator : public Allocator {
    public:
        PoolAllocator(std::size_t total, const std::size_t chunk);
        PoolAllocator(const PoolAllocator& allocator) = delete;
        PoolAllocator(PoolAllocator&& allocator) = delete;
        virtual ~PoolAllocator();

        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
        virtual void Free(void* ptr) override;

        virtual void Init() override;
        virtual void Reset() override;

    private:
        struct FreeHeader{};
        using Node = StackLinkedList<FreeHeader>::Node;
        StackLinkedList<FreeHeader> free_list;

        void * start_ptr = nullptr;
        std::size_t chunk_size = 0;
    };
}
