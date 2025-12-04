// 15. Implement  Prim's and Kruskal’s algorithm to find the minimum spanning tree of a graph. 

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int findMinVertex(vector<int>& key, vector<bool>& mstSet, int V) {
    int minKey = INT_MAX, minIndex = -1;
    for (int i = 0; i < V; i++) {
        if (!mstSet[i] && key[i] < minKey) {
            minKey = key[i];
            minIndex = i;
        }
    }
    return minIndex;
}

void primMST(vector<vector<int>>& graph, int V) {
    vector<int> parent(V);
    vector<int> key(V, INT_MAX);
    vector<bool> mstSet(V, false);

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = findMinVertex(key, mstSet, V);
        mstSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    cout << "\nPrim's MST:\nEdge  Weight\n";
    for (int i = 1; i < V; i++)
        cout << parent[i] << " - " << i << "   " << graph[i][parent[i]] << endl;
}

int main() {
    int V;
    cout << "Enter number of vertices: ";
    cin >> V;

    vector<vector<int>> graph(V, vector<int>(V));

    cout << "Enter adjacency matrix of weighted graph:\n";
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            cin >> graph[i][j];

    primMST(graph, V);
    return 0;
}
