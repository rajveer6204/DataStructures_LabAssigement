// 14. Perform the traversal of graph(DFS,BFS)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Graph {
public:
    int V;    
    vector<vector<int>> adj;

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Add edge (Undirected)
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // DFS helper (recursive)
    void DFSUtil(int node, vector<bool>& visited) {
        visited[node] = true;
        cout << node << " ";

        for (int neighbor : adj[node]) {
            if (!visited[neighbor])
                DFSUtil(neighbor, visited);
        }
    }

    // DFS Traversal
    void DFS(int start) {
        vector<bool> visited(V, false);
        cout << "DFS Traversal starting from " << start << ": ";
        DFSUtil(start, visited);
        cout << endl;
    }

    // BFS Traversal
    void BFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS Traversal starting from " << start << ": ";

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            cout << node << " ";

            for (int neighbor : adj[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;

    Graph g(V);

    cout << "Enter number of edges: ";
    cin >> E;

    cout << "Enter edges (u v):" << endl;
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter start node for traversal: ";
    cin >> start;

    g.DFS(start);
    g.BFS(start);

    return 0;
}
