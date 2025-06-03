#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    vector<pair<int, int>> edges;

    for (int i = 2; i <= n; i++) {
        int row_index = i - 1;
        int parent = 0;
        for (int j = 0; j < i - 1; j++) {
            if (matrix[row_index][j] > parent) {
                parent = matrix[row_index][j];
            }
        }
        edges.push_back(make_pair(parent, i));
    }

    sort(edges.begin(), edges.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        if (a.first != b.first) {
            return a.first > b.first;
        }
        return a.second < b.second;
    });

    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << endl;
    }

    return 0;
}