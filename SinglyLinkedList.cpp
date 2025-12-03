//2. Write a program to implement a singly linked list with methods to insert an element at the head, insert an element at the tail, delete an element by value, and traverse the list to print all elements.

#include <iostream>
using namespace std;

// Node structure
class Node {
public:
    int data;
    Node* next;

    Node(int value) {
        data = value;
        next = NULL;
    }
};

// Singly Linked List class
class LinkedList {
private:
    Node* head;

public:
    LinkedList() {
        head = NULL;
    }

    // Insert at head
    void insertAtHead(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    // Insert at tail
    void insertAtTail(int value) {
        Node* newNode = new Node(value);

        if (head == NULL) {
            head = newNode;
            return;
        }

        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Delete by value
    void deleteByValue(int value) {
        if (head == NULL) {
            cout << "List is empty. Nothing to delete.\n";
            return;
        }

        // If the head node holds the value
        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* current = head;
        Node* prev = NULL;

        while (current != NULL && current->data != value) {
            prev = current;
            current = current->next;
        }

        // Value not found
        if (current == NULL) {
            cout << "Value not found in the list.\n";
            return;
        }

        // Delete the node
        prev->next = current->next;
        delete current;
    }

    // Traverse / Print list
    void traverse() {
        Node* temp = head;

        if (temp == NULL) {
            cout << "List is empty.\n";
            return;
        }

        cout << "Linked List: ";
        while (temp != NULL) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

// Main function
int main() {
    LinkedList list;

    list.insertAtHead(10);
    list.insertAtHead(20);
    list.insertAtTail(30);
    list.insertAtTail(40);

    list.traverse();

    cout << "Deleting 20...\n";
    list.deleteByValue(20);
    list.traverse();

    cout << "Deleting 40...\n";
    list.deleteByValue(40);
    list.traverse();

    return 0;
}
