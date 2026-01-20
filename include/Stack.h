#ifndef STACK_H
#define STACK_H

#include "Node.h"

// ============================================================================
// STACK TEMPLATE CLASS (LIFO - Last In First Out)
// ============================================================================
template <typename T>
class Stack {
private:
    Node<T>* top;
    int size;

public:
    // Constructor
    Stack() : top(nullptr), size(0) {}
    
    // Destructor
    ~Stack() {
        clear();
    }
    
    // ========================================================================
    // STACK OPERATIONS
    // ========================================================================
    
    // Push element onto stack
    void push(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    // Pop element from stack
    void pop() {
        if (top == nullptr) throw "Stack underflow: cannot pop from empty stack";
        
        Node<T>* temp = top;
        top = top->next;
        delete temp;
        size--;
    }
    
    // Get top element
    T& peek() const {
        if (top == nullptr) throw "Stack is empty";
        return top->data;
    }
    
    // Check if empty
    bool isEmpty() const {
        return size == 0;
    }
    
    // Get size
    int getSize() const {
        return size;
    }
    
    // Clear the stack
    void clear() {
        while (top != nullptr) {
            pop();
        }
    }
    
    // ========================================================================
    // UTILITY OPERATIONS FOR ROLLBACK
    // ========================================================================
    
    // Get top node for potential inspection
    Node<T>* getTopNode() const {
        return top;
    }
    
    // Count elements in stack
    int count() const {
        return size;
    }
};

#endif // STACK_H
