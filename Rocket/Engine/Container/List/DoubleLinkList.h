#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    template <class T>
    class DoubleLinkList {
    public:
        struct Node {
            T data;
            Node* previous;
            Node* next;
        };
        Node* head;
    public:
        DoubleLinkList() = default;
        DoubleLinkList(const DoubleLinkList& doublyLinkedList) = delete;
        DoubleLinkList& operator = (const DoubleLinkList& other) = delete;
        DoubleLinkList& operator = (DoubleLinkList&& stackLinkedList) {
            this->head = stackLinkedList.head;
            stackLinkedList.head = nullptr;
        }
        DoubleLinkList(DoubleLinkList&& stackLinkedList) {
            this->head = stackLinkedList.head;
            stackLinkedList.head = nullptr;
        }

        void insert(Node* previousNode, Node* newNode);
        void remove(Node* deleteNode);
    };

    template <class T>
    void DoubleLinkList<T>::insert(Node* previousNode, Node* newNode) {
        if (previousNode == nullptr) {
            // Is the first node
            if (head != nullptr) {
                // The list has more elements
                newNode->next = head;           
                newNode->next->previous = newNode;
            }else {
                newNode->next = nullptr;
            }
            head = newNode;
            head->previous = nullptr;
        } else {
            if (previousNode->next == nullptr){
                // Is the last node
                previousNode->next = newNode;
                newNode->next = nullptr;
            }else {
                // Is a middle node
                newNode->next = previousNode->next;
                if (newNode->next != nullptr){
                    newNode->next->previous = newNode;
                }
                previousNode->next = newNode;
                newNode->previous = previousNode;
            }
        }
    }

    template <class T>
    void DoubleLinkList<T>::remove(Node* deleteNode) {
        if (deleteNode->previous == nullptr){
            // Is the first node
            if (deleteNode->next == nullptr){
                // List only has one element
                head = nullptr;            
            }else {
                // List has more elements
                head = deleteNode->next;
                head->previous = nullptr;
            }
        }else {
            if (deleteNode->next == nullptr){
                // Is the last node
                deleteNode->previous->next = nullptr;
            }else {
                // Middle node
                deleteNode->previous->next = deleteNode->next;
                deleteNode->next->previous = deleteNode->previous;
            }
        }
    }
}
