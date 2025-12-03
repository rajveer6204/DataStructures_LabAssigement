//  8. Create a circular queue using array with methods for enqueue, dequeue, and checking if the queue is empty or full. Ensure that the circular nature of the queue is maintained after each operation.

#include <iostream>
using namespace std;

class CircularQueue {
private:
    int size;
    int front;
    int rear;
    int *arr;

public:
    CircularQueue(int s) {
        size = s;
        arr = new int[size];
        front = rear = -1;
    }

    bool isFull() {
        return (front == (rear + 1) % size);
    }

    bool isEmpty() {
        return (front == -1);
    }

    void enqueue(int value) {
        if (isFull()) {
            cout << "Queue is full! Cannot enqueue.\n";
            return;
        }
        if (isEmpty()) {
            front = rear = 0;
        } else {
            rear = (rear + 1) % size;
        }
        arr[rear] = value;
        cout << value << " enqueued.\n";
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty! Cannot dequeue.\n";
            return;
        }
        int removed = arr[front];
        if (front == rear) { 
            // Only one element
            front = rear = -1;
        } else {
            front = (front + 1) % size;
        }
        cout << removed << " dequeued.\n";
    }

    void display() {
        if (isEmpty()) {
            cout << "Queue is empty.\n";
            return;
        }

        cout << "Circular Queue elements: ";
        int i = front;
        while (true) {
            cout << arr[i] << " ";
            if (i == rear)
                break;
            i = (i + 1) % size;
        }
        cout << endl;
    }
};

int main() {
    CircularQueue q(5);

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.display();

    q.dequeue();
    q.dequeue();
    q.display();

    q.enqueue(50);
    q.enqueue(60);
    q.enqueue(70); // wraps around
    q.display();

    return 0;
}
