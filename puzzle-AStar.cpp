#include <iostream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    string state;
    int g;
    int h;
    int f;

    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

// Calculate heuristic h(n)
// = number of misplaced tiles
int heuristic(string state, string goal) {

    int count = 0;

    for (int i = 0; i < 9; i++) {

        // Do not count the empty cell
        if (state[i] != '0' &&
            state[i] != goal[i]) {

            count++;
        }
    }

    return count;
}

// Generate neighbouring states
vector<string> getNeighbors(string state) {

    vector<string> neighbors;

    int zeroPos = state.find('0');

    int row = zeroPos / 3;
    int col = zeroPos % 3;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {

        int newRow = row + dr[i];
        int newCol = col + dc[i];

        if (newRow >= 0 && newRow < 3 &&
            newCol >= 0 && newCol < 3) {

            string next = state;

            int newPos = newRow * 3 + newCol;

            swap(next[zeroPos], next[newPos]);

            neighbors.push_back(next);
        }
    }

    return neighbors;
}

// Print board
void printBoard(string state) {

    for (int i = 0; i < 9; i++) {

        if (state[i] == '0')
            cout << "# ";
        else
            cout << state[i] << " ";

        if (i % 3 == 2)
            cout << endl;
    }

    cout << endl;
}

int main() {

    string start = "123046758";
    string goal  = "123456780";

    priority_queue<Node,
                   vector<Node>,
                   greater<Node>> pq;

    unordered_map<string, int> bestG;

    unordered_map<string, string> parent;

    // Initial node
    int h = heuristic(start, goal);

    pq.push({start, 0, h, 0 + h});

    bestG[start] = 0;

    while (!pq.empty()) {

        Node current = pq.top();
        pq.pop();

        string state = current.state;

        // Ignore outdated node
        if (current.g != bestG[state])
            continue;

        // Goal reached
        if (state == goal)
            break;

        vector<string> neighbors = getNeighbors(state);

        for (string next : neighbors) {

            // g(next) = g(current) + 1
            int newG = current.g + 1;

            // If this is a better path
            if (bestG.find(next) == bestG.end() ||
                newG < bestG[next]) {

                bestG[next] = newG;

                int newH = heuristic(next, goal);

                int newF = newG + newH;

                parent[next] = state;

                pq.push({
                    next,
                    newG,
                    newH,
                    newF
                });
            }
        }
    }

    // Construct solution path
    vector<string> path;

    string current = goal;

    while (current != start) {

        path.push_back(current);

        current = parent[current];
    }

    path.push_back(start);

    reverse(path.begin(), path.end());

    cout << "A* Solution\n";
    cout << "Number of moves = " << path.size() - 1 << "\n\n";

    for (int i = 0; i < path.size(); i++) {

        cout << "Step " << i << ":\n";

        printBoard(path[i]);

        cout << "g = " << i
             << ", h = " << heuristic(path[i], goal)
             << ", f = " << i + heuristic(path[i], goal)
             << "\n\n";
    }

    return 0;
}