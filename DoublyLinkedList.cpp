// 4.Implement a doubly linked list with methods to insert an element at the head, insert an element at the tail, delete an element by value, and reverse the list. Ensure that all operations handle edge cases appropriately.

#include <iostream>
using namespace std;

// Node structure for Doubly Linked List
class Node {
public:
    int data;
    Node* prev;
    Node* next;

    Node(int value) {
        data = value;
        prev = NULL;
        next = NULL;
    }
};

// Doubly Linked List class
class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() {
        head = NULL;
        tail = NULL;
    }

    // Insert at Head
    void insertAtHead(int value) {
        Node* newNode = new Node(value);

        if (head == NULL) {  // Empty list
            head = tail = newNode;
            return;
        }

        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    // Insert at Tail
    void insertAtTail(int value) {
        Node* newNode = new Node(value);

        if (tail == NULL) {  // Empty list
            head = tail = newNode;
            return;
        }

        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    // Delete by value
    void deleteByValue(int value) {
        if (head == NULL) {
            cout << "List is empty.\n";
            return;
        }

        Node* temp = head;

        // Search for the node
        while (temp != NULL && temp->data != value) {
            temp = temp->next;
        }

        if (temp == NULL) {
            cout << "Value not found.\n";
            return;
        }

        // Case 1: Only one node
        if (head == tail) {
            delete temp;
            head = tail = NULL;
            return;
        }

        // Case 2: Delete head
        if (temp == head) {
            head = head->next;
            head->prev = NULL;
            delete temp;
            return;
        }

        // Case 3: Delete tail
        if (temp == tail) {
            tail = tail->prev;
            tail->next = NULL;
            delete temp;
            return;
        }

        // Case 4: Delete in middle
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
    }

    // Reverse the doubly linked list
    void reverse() {
        if (head == NULL || head->next == NULL)
            return;

        Node* current = head;
        Node* temp = NULL;

        // Swap next and prev for all nodes
        while (current != NULL) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;

            current = current->prev; // move to next node (which is previous originally)
        }

        // After loop, temp is at old head
        head = temp->prev;
    }

    // Traverse the list
    void traverse() {
        Node* temp = head;
        while (temp != NULL) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

// Main function
int main() {
    DoublyLinkedList dll;

    dll.insertAtHead(10);
    dll.insertAtHead(20);
    dll.insertAtTail(5);
    dll.insertAtTail(1);

    cout << "List: ";
    dll.traverse();

    dll.deleteByValue(10);
    cout << "After deleting 10: ";
    dll.traverse();

    dll.reverse();
    cout << "After reversing: ";
    dll.traverse();

    return 0;
}
