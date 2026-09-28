#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

int main() {
    // Graph: {node, edge cost}
    vector<vector<pair<int, int>>> graph(6);

    // a=0, b=1, c=2, d=3, e=4, f=5

    graph[0].push_back({1, 5}); // a-b
    graph[0].push_back({2, 2}); // a-c
    graph[0].push_back({3, 3}); // a-d

    graph[1].push_back({0, 5}); // b-a
    graph[1].push_back({2, 2}); // b-c
    graph[1].push_back({5, 3}); // b-f

    graph[2].push_back({0, 2}); // c-a
    graph[2].push_back({1, 2}); // c-b
    graph[2].push_back({3, 1}); // c-d
    graph[2].push_back({4, 2}); // c-e
    graph[2].push_back({5, 6}); // c-f

    graph[3].push_back({0, 3}); // d-a
    graph[3].push_back({2, 1}); // d-c
    graph[3].push_back({4, 4}); // d-e

    graph[4].push_back({3, 4}); // e-d
    graph[4].push_back({2, 2}); // e-c
    graph[4].push_back({5, 4}); // e-f

    graph[5].push_back({1, 3}); // f-b
    graph[5].push_back({2, 6}); // f-c
    graph[5].push_back({4, 4}); // f-e

    int start = 0; // a
    int goal = 5;  // f

    vector<int> dist(6, INT_MAX);
    vector<int> parent(6, -1);

    // Min priority queue: {distance, node}
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Ignore outdated entry
        if (currentDist != dist[u])
            continue;

        for (auto edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;

                pq.push({dist[v], v});
            }
        }
    }

    // Construct path
    vector<int> path;

    for (int v = goal; v != -1; v = parent[v]) {
        path.push_back(v);
    }

    reverse(path.begin(), path.end());

    cout << "Dijkstra's Algorithm\n";
    cout << "Shortest Path: ";

    for (int i = 0; i < path.size(); i++) {
        cout << char('a' + path[i]);

        if (i != path.size() - 1)
            cout << " -> ";
    }

    cout << "\nShortest Cost: " << dist[goal] << endl;

    return 0;
}