#pragma once
#include "Core/MemoryDefine.h"

namespace Rocket {
    template <class T>
    class SingleLinkList {
    public:
        struct Node {
            T data;
            Node * next;
        };
        
        Node * head;
        
    public:
        SingleLinkList() = default;
        SingleLinkList(const SingleLinkList& doublyLinkedList) = delete;
        SingleLinkList(SingleLinkList&& stackLinkedList) {
            this->head = stackLinkedList.head;
            stackLinkedList.head = nullptr;
        }

        void insert(Node * previousNode, Node * newNode);
        void remove(Node * previousNode, Node * deleteNode);
    };

    template <class T>
    void SingleLinkList<T>::insert(Node* previousNode, Node* newNode){
        if (previousNode == nullptr) {
            // Is the first node
            if (head != nullptr) {
                // The list has more elements
                newNode->next = head;           
            }else {
                newNode->next = nullptr;
            }
            head = newNode;
        } else {
            if (previousNode->next == nullptr){
                // Is the last node
                previousNode->next = newNode;
                newNode->next = nullptr;
            }else {
                // Is a middle node
                newNode->next = previousNode->next;
                previousNode->next = newNode;
            }
        }
    }

    template <class T>
    void SingleLinkList<T>::remove(Node* previousNode, Node* deleteNode){
        if (previousNode == nullptr){
            // Is the first node
            if (deleteNode->next == nullptr){
                // List only has one element
                head = nullptr;            
            }else {
                // List has more elements
                head = deleteNode->next;
            }
        }else {
            previousNode->next = deleteNode->next;
        }
    }
    }
