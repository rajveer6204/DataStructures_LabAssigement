// 1. Given an array of integers, perform the following operations: traversing , insertion, deletion

#include <iostream>
using namespace std;

// Function to traverse and print array
void traverse(int arr[], int n) {
    cout << "Array elements: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Function to insert element at given position
void insertElement(int arr[], int &n, int pos, int value) {
    // Shift elements to the right
    for (int i = n; i > pos; i--) {
        arr[i] = arr[i - 1];
    }
    arr[pos] = value;
    n++;
}

// Function to delete element from given position
void deleteElement(int arr[], int &n, int pos) {
    // Shift elements to the left
    for (int i = pos; i < n - 1; i++) {
        arr[i] = arr[i + 1];
    }
    n--;
}

int main() {
    int arr[100] = {10, 20, 30, 40, 50};
    int n = 5;

    cout << "Initial Array:\n";
    traverse(arr, n);

    // Insert example
    int insertPos = 2;
    int insertValue = 99;
    insertElement(arr, n, insertPos, insertValue);
    cout << "\nAfter Insertion of 99 at position 2:\n";
    traverse(arr, n);

    // Delete example
    int deletePos = 3;
    deleteElement(arr, n, deletePos);
    cout << "\nAfter Deletion at position 3:\n";
    traverse(arr, n);

    return 0;
}
