// 7. Create a linear queue using an array with methods for enqueue, dequeue, and checking if the queue is empty or full. 

#include <iostream>
using namespace std;

class Queue {
private:
    int size;
    int front;
    int rear;
    int *arr;

public:
    // Constructor
    Queue(int s) {
        size = s;
        arr = new int[size];
        front = -1;
        rear = -1;
    }

    // Check if queue is empty
    bool isEmpty() {
        return (front == -1 || front > rear);
    }

    // Check if queue is full
    bool isFull() {
        return (rear == size - 1);
    }

    // Enqueue operation
    void enqueue(int value) {
        if (isFull()) {
            cout << "Queue Overflow! Cannot enqueue.\n";
            return;
        }
        if (front == -1) {
            front = 0;  // first element
        }
        rear++;
        arr[rear] = value;
        cout << value << " enqueued.\n";
    }

    // Dequeue operation
    int dequeue() {
        if (isEmpty()) {
            cout << "Queue Underflow! Cannot dequeue.\n";
            return -1;
        }
        int removed = arr[front];
        front++;
        cout << removed << " dequeued.\n";
        return removed;
    }

    // Display queue
    void display() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }
        cout << "Queue elements: ";
        for (int i = front; i <= rear; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    Queue q(5);

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);

    q.display();

    q.dequeue();
    q.display();

    q.enqueue(50);
    q.enqueue(60); 
    q.display();

    return 0;
}
