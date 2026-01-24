#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"

// ============================================================================
// DOUBLY LINKED LIST TEMPLATE CLASS
// ============================================================================
template <typename T>
class DoublyLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    // Constructor
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Deep copy and move support to avoid double frees when returning by value
    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current != nullptr) {
            insertBack(current->data);
            current = current->next;
        }
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current != nullptr) {
                insertBack(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head(other.head), tail(other.tail), size(other.size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            size = other.size;
            other.head = nullptr;
            other.tail = nullptr;
            other.size = 0;
        }
        return *this;
    }
    
    // Destructor
    ~DoublyLinkedList() {
        clear();
    }
    
    // ========================================================================
    // BASIC OPERATIONS
    // ========================================================================
    
    // Insert at the beginning
    void insertFront(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }
    
    // Insert at the end
    void insertBack(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }
    
    // Remove from the beginning
    void removeFront() {
        if (head == nullptr) return;
        
        Node<T>* temp = head;
        head = head->next;
        
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        
        delete temp;
        size--;
    }
    
    // Remove from the end
    void removeBack() {
        if (tail == nullptr) return;
        
        Node<T>* temp = tail;
        tail = tail->prev;
        
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        
        delete temp;
        size--;
    }
    
    // Get front element
    T& getFront() const {
        if (head == nullptr) throw "List is empty";
        return head->data;
    }
    
    // Get back element
    T& getBack() const {
        if (tail == nullptr) throw "List is empty";
        return tail->data;
    }
    
    // ========================================================================
    // SEARCH AND ACCESS OPERATIONS
    // ========================================================================
    
    // Find element by value (requires operator== for type T)
    Node<T>* find(const T& value) const {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // Get element at index
    T& getAt(int index) const {
        if (index < 0 || index >= size) throw "Index out of bounds";
        
        Node<T>* current = nullptr;
        
        // Optimize by choosing nearest end
        if (index < size / 2) {
            current = head;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
        } else {
            current = tail;
            for (int i = size - 1; i > index; i--) {
                current = current->prev;
            }
        }
        
        return current->data;
    }
    
    // ========================================================================
    // UTILITY OPERATIONS
    // ========================================================================
    
    // Get size
    int getSize() const {
        return size;
    }
    
    // Check if empty
    bool isEmpty() const {
        return size == 0;
    }
    
    // Clear the list
    void clear() {
        while (head != nullptr) {
            removeFront();
        }
    }
    
    // Remove a specific node
    void removeNode(Node<T>* node) {
        if (node == nullptr) return;
        
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
        
        delete node;
        size--;
    }
    
    // ========================================================================
    // ITERATION SUPPORT
    // ========================================================================
    
    // Get head pointer for manual iteration
    Node<T>* getHead() const {
        return head;
    }
    
    // Get tail pointer for manual iteration
    Node<T>* getTail() const {
        return tail;
    }
    
    // Print the list (requires operator<< for type T)
    void printForward() const {
        Node<T>* current = head;
        while (current != nullptr) {
            // Requires overload: std::cout << current->data;
            current = current->next;
        }
    }
};

#endif // LINKEDLIST_H
