#ifndef NODE_H
#define NODE_H

// ============================================================================
// DOUBLY LINKED LIST NODE
// ============================================================================
template <typename T>
struct Node {
    T data;
    Node<T>* next;
    Node<T>* prev;
    
    // Constructor
    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    
    // Destructor
    ~Node() {}
};

#endif // NODE_H
