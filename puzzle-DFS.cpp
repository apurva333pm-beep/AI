#include <iostream>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

string boardToString(const vector<vector<int>>& board) {
    string s;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            s += char(board[i][j] + '0');
        }
    }

    return s;
}

vector<vector<int>> stringToBoard(string s) {
    vector<vector<int>> board(3, vector<int>(3));

    for (int i = 0; i < 9; i++) {
        board[i / 3][i % 3] = s[i] - '0';
    }

    return board;
}

void printBoard(string state) {
    vector<vector<int>> board = stringToBoard(state);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (board[i][j] == 0)
                cout << "# ";
            else
                cout << board[i][j] << " ";
        }

        cout << endl;
    }

    cout << endl;
}

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

int main() {

    string start = "123046758";
    string goal  = "123456780";

    stack<string> st;

    unordered_set<string> visited;

    unordered_map<string, string> parent;

    st.push(start);
    visited.insert(start);

    while (!st.empty()) {

        string current = st.top();
        st.pop();

        if (current == goal)
            break;

        vector<string> neighbors = getNeighbors(current);

        // Reverse so that the first generated move
        // is explored first by the stack.
        reverse(neighbors.begin(), neighbors.end());

        for (string next : neighbors) {

            if (visited.find(next) == visited.end()) {

                visited.insert(next);

                parent[next] = current;

                st.push(next);
            }
        }
    }

    // Construct path
    vector<string> path;

    string current = goal;

    while (current != start) {

        path.push_back(current);

        current = parent[current];
    }

    path.push_back(start);

    reverse(path.begin(), path.end());

    cout << "DFS Solution\n";
    cout << "Number of moves = " << path.size() - 1 << "\n\n";

    for (int i = 0; i < path.size(); i++) {

        cout << "Step " << i << ":\n";

        printBoard(path[i]);
    }

    return 0;
}