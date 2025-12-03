//10.Implement binary search(iterative and recursive )

#include <iostream>
using namespace std;

// Recursive Binary Search
int binarySearchRecursive(int arr[], int left, int right, int key) {
    if (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;

        if (arr[mid] > key)
            return binarySearchRecursive(arr, left, mid - 1, key);

        return binarySearchRecursive(arr, mid + 1, right, key);
    }
    return -1; // Not found
}

// Iterative Binary Search
int binarySearchIterative(int arr[], int n, int key) {
    int left = 0, right = n - 1;

    while (left <= right) {

        int mid = left + (right - left) / 2;

        if (arr[mid] == key)
            return mid;

        if (arr[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1; // Not found
}

int main() {
    int arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72};
    int n = sizeof(arr) / sizeof(arr[0]);

    int key;
    cout << "Enter element to search: ";
    cin >> key;

    int resultIter = binarySearchIterative(arr, n, key);
    int resultRec = binarySearchRecursive(arr, 0, n - 1, key);

    cout << "\n--- Binary Search Results ---\n";

    if (resultIter != -1)
        cout << "Iterative: Element found at index " << resultIter << endl;
    else
        cout << "Iterative: Element not found\n";

    if (resultRec != -1)
        cout << "Recursive: Element found at index " << resultRec << endl;
    else
        cout << "Recursive: Element not found\n";

    return 0;
}
