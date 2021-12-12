#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    template <class T>
    class StackLinkList {
    public:
        struct Node {
            T data;
            Node* next;
        };
        
        Node* head;
    public:
        StackLinkList() = default;
        StackLinkList(const StackLinkList& stackLinkedList) = delete;
        StackLinkList(StackLinkList&& stackLinkedList) {
            this->head = stackLinkedList.head;
            stackLinkedList.head = nullptr;
        }
        void push(Node* new_node);
        Node* pop();
    };

    template <class T>
    void StackLinkList<T>::push(Node* new_node) {
        new_node->next = head;
        head = new_node;
    }

    template <class T>
    typename StackLinkList<T>::Node* StackLinkList<T>::pop() {
        Node* top = head;
        head = head->next;
        return top;
    }
}
