#pragma once

namespace Rocket {
    template<typename T>
    class List {
        struct Node {
            Node* next;
            T data;
        };
    public:
        List();

    private:
        Node* root_ = nullptr;
    };
}
