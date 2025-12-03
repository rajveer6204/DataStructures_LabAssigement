//3. Write a class to implement a circular linked list with methods to insert an element at the head, insert an element at the tail, delete an element by value, and traverse the list to print all elements.

#include <iostream>
using namespace std;

// Node structure
class Node {
public:
    int data;
    Node* next;

    Node(int value) {
        data = value;
        next = nullptr;
    }
};

// Circular Linked List class
class CircularLinkedList {
private:
    Node* head;

public:
    // Constructor
    CircularLinkedList() {
        head = nullptr;
    }

    // Insert at head
    void insertAtHead(int value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = newNode;
            newNode->next = head;   // Point to itself
            return;
        }

        Node* temp = head;

        // Move to last node
        while (temp->next != head) {
            temp = temp->next;
        }

        newNode->next = head;
        temp->next = newNode;
        head = newNode;
    }

    // Insert at tail
    void insertAtTail(int value) {
        Node* newNode = new Node(value);

        if (head == nullptr) {
            head = newNode;
            newNode->next = head;
            return;
        }

        Node* temp = head;

        // Find last node
        while (temp->next != head) {
            temp = temp->next;
        }

        temp->next = newNode;
        newNode->next = head;
    }

    // Delete element by value
    void deleteByValue(int value) {
        if (head == nullptr) {
            cout << "List is empty.\n";
            return;
        }

        Node* temp = head;
        Node* prev = nullptr;

        // Special case: deleting head
        if (temp->data == value) {
            // Only one node
            if (temp->next == head) {
                delete temp;
                head = nullptr;
                return;
            }

            // Move to last node
            Node* last = head;
            while (last->next != head) {
                last = last->next;
            }

            last->next = head->next;
            head = head->next;

            delete temp;
            return;
        }

        // Search for the node to delete
        do {
            prev = temp;
            temp = temp->next;

            if (temp->data == value) {
                prev->next = temp->next;
                delete temp;
                return;
            }

        } while (temp != head);

        cout << "Value not found in list.\n";
    }

    // Traverse and print
    void traverse() {
        if (head == nullptr) {
            cout << "List is empty.\n";
            return;
        }

        Node* temp = head;

        cout << "Circular Linked List: ";
        do {
            cout << temp->data << " ";
            temp = temp->next;
        } while (temp != head);

        cout << endl;
    }
};

// Main function to test the class
int main() {
    CircularLinkedList cll;

    cll.insertAtHead(10);
    cll.insertAtHead(20);
    cll.insertAtTail(30);
    cll.insertAtTail(40);

    cll.traverse();

    cll.deleteByValue(20);
    cll.traverse();

    cll.deleteByValue(40);
    cll.traverse();

    return 0;
}
