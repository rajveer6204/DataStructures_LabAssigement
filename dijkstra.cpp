#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
using namespace std;

// ------------------------
// Dijkstra Using Adjacency Matrix
// ------------------------
void dijkstraMatrix(vector<vector<int>>& graph, int src) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    vector<bool> visited(n, false);

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        }

        visited[u] = true;

        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0 && dist[u] != INT_MAX &&
                dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    cout << "\nShortest distance from source using Adjacency Matrix:\n";
    for (int i = 0; i < n; i++)
        cout << "Node " << i << " : " << dist[i] << endl;
}

// ------------------------
// Dijkstra Using Adjacency List
// ------------------------
void dijkstraList(vector<vector<pair<int,int>>>& adj, int src) {
    int n = adj.size();
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    pq.push({0, src});
    dist[src] = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (auto &edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }

    cout << "\nShortest distance from source using Adjacency List:\n";
    for (int i = 0; i < n; i++)
        cout << "Node " << i << " : " << dist[i] << endl;
}

int main() {
    // SAMPLE GRAPH (NO NEGATIVE WEIGHTS)
    // Graph with 5 nodes
    vector<vector<int>> matrix = {
        {0, 10, 0, 30, 100},
        {10, 0, 50, 0, 0},
        {0, 50, 0, 20, 10},
        {30, 0, 20, 0, 60},
        {100, 0, 10, 60, 0}
    };

    vector<vector<pair<int,int>>> adjList(5);

    adjList[0] = {{1,10}, {3,30}, {4,100}};
    adjList[1] = {{0,10}, {2,50}};
    adjList[2] = {{1,50}, {3,20}, {4,10}};
    adjList[3] = {{0,30}, {2,20}, {4,60}};
    adjList[4] = {{0,100}, {2,10}, {3,60}};

    int src = 0;

    dijkstraMatrix(matrix, src);
    dijkstraList(adjList, src);

    return 0;
}
