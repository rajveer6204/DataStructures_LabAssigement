// 9.Implement linear search 

#include <iostream>
using namespace std;

int linearSearch(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return i; // Return index of the element
        }
    }
    return -1; // Element not found
}

int main() {
    int n, key;

    cout << "Enter number of elements: ";
    cin >> n;

    int arr[n];
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << "Enter element to search: ";
    cin >> key;

    int result = linearSearch(arr, n, key);

    if (result != -1)
        cout << "Element found at index " << result << endl;
    else
        cout << "Element not found!" << endl;

    return 0;
}
