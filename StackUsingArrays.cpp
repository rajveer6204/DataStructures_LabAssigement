// 5.Implement a stack using arrays with methods for push, pop, and peek operations. 

#include <iostream>
using namespace std;

class Stack {
private:
    int arr[100];   // Fixed-size array (capacity = 100)
    int top;        // Index of the top element
    int capacity;   // Maximum size of stack

public:
    // Constructor
    Stack(int size = 100) {
        capacity = size;
        top = -1;   // Stack is empty
    }

    // Push operation
    void push(int value) {
        if (top == capacity - 1) {
            cout << "Stack Overflow! Cannot push " << value << endl;
            return;
        }
        top++;
        arr[top] = value;
        cout << value << " pushed to stack." << endl;
    }

    // Pop operation
    int pop() {
        if (top == -1) {
            cout << "Stack Underflow! Cannot pop." << endl;
            return -1;
        }
        int popped = arr[top];
        top--;
        cout << popped << " popped from stack." << endl;
        return popped;
    }

    // Peek operation
    int peek() {
        if (top == -1) {
            cout << "Stack is empty!" << endl;
            return -1;
        }
        return arr[top];
    }

    // Display stack
    void display() {
        if (top == -1) {
            cout << "Stack is empty!" << endl;
            return;
        }
        cout << "Stack elements: ";
        for (int i = 0; i <= top; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    Stack s(5);  // Create stack of size 5

    s.push(10);
    s.push(20);
    s.push(30);

    cout << "Top element: " << s.peek() << endl;

    s.pop();
    s.display();

    s.push(40);
    s.push(50);
    s.push(60);  // This will overflow if size < 6

    return 0;
}
