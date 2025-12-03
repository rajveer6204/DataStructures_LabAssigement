// 13.Given preorder and inorder traversal of a tree, construct the binary tree. 

#include <iostream>
#include <unordered_map>
using namespace std;

// Tree Node
struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = right = nullptr;
    }
};

// Build tree function
Node* buildTreeHelper(int preorder[], int inorder[], int start, int end,
                      int& preorderIndex, unordered_map<int, int>& inorderIndexMap) {

    if (start > end)
        return nullptr;

    // Pick current node from preorder traversal
    int curr = preorder[preorderIndex++];
    Node* node = new Node(curr);

    // If node has no children
    if (start == end)
        return node;

    // Find index of current root in inorder
    int position = inorderIndexMap[curr];

    // Recursively build left and right subtree
    node->left  = buildTreeHelper(preorder, inorder, start, position - 1, preorderIndex, inorderIndexMap);
    node->right = buildTreeHelper(preorder, inorder, position + 1, end, preorderIndex, inorderIndexMap);

    return node;
}

// Main build function
Node* buildTree(int preorder[], int inorder[], int n) {
    unordered_map<int, int> inorderIndexMap;

    for (int i = 0; i < n; i++)
        inorderIndexMap[inorder[i]] = i;

    int preorderIndex = 0;
    return buildTreeHelper(preorder, inorder, 0, n - 1, preorderIndex, inorderIndexMap);
}

// Print tree in Postorder to verify
void printPostorder(Node* root) {
    if (!root) return;

    printPostorder(root->left);
    printPostorder(root->right);
    cout << root->data << " ";
}

// Driver Code
int main() {
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;

    int preorder[n], inorder[n];
    
    cout << "Enter preorder traversal: ";
    for (int i = 0; i < n; i++)
        cin >> preorder[i];

    cout << "Enter inorder traversal: ";
    for (int i = 0; i < n; i++)
        cin >> inorder[i];

    Node* root = buildTree(preorder, inorder, n);

    cout << "\nPostorder traversal of the constructed tree: ";
    printPostorder(root);
    cout << endl;

    return 0;
}
