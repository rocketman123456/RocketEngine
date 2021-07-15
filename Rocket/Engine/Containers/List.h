#pragma once

namespace Rocket {
    template<typename T>
    class List {
        struct Node {
            Node* next;
            T data;
        };
    public:
        explicit List();
        virtual ~List();

    private:
        Node* first_ = nullptr;
    };
}
