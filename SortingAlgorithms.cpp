// 12. Implement various sorting algorithms including Quick Sort, Merge Sort, Heap Sort, and analyze their performance on different input sizes. Ensure the implementation handles edge cases such as duplicate values and nearly sorted arrays.

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

// Function to print an array
void printArray(vector<int>& arr) {
    for (int i : arr) {
        cout << i << " ";
    }
    cout << endl;
}

// Insertion Sort
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        // Move elements greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Selection Sort
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex])
                minIndex = j;
        }
        swap(arr[i], arr[minIndex]);
    }
}

// Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break; // Array already sorted
    }
}

int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
    vector<int> arr(n);
    
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;

    // Insertion Sort
    auto start = high_resolution_clock::now();
    insertionSort(arr1);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Insertion Sort Result: ";
    printArray(arr1);
    cout << "Time taken by Insertion Sort: " << duration.count() << " microseconds" << endl;

    // Selection Sort
    start = high_resolution_clock::now();
    selectionSort(arr2);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Selection Sort Result: ";
    printArray(arr2);
    cout << "Time taken by Selection Sort: " << duration.count() << " microseconds" << endl;

    // Bubble Sort
    start = high_resolution_clock::now();
    bubbleSort(arr3);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Bubble Sort Result: ";
    printArray(arr3);
    cout << "Time taken by Bubble Sort: " << duration.count() << " microseconds" << endl;

    return 0;
}
