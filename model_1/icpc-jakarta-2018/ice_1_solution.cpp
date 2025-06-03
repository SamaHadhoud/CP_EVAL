#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int MAX = 505;

bool dfs(int u, bool seen[], vector<int> graph[], int matchR[]) {
    for (int v : graph[u]) {
        if (!seen[v]) {
            seen[v] = true;
            if (matchR[v] == -1 || dfs(matchR[v], seen, graph, matchR)) {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

int main() {
    int R, C;
    cin >> R >> C;
    char grid[MAX][MAX];
    for (int i = 0; i < R; i++) {
        cin >> grid[i];
    }

    bool rowEmpty[MAX], colEmpty[MAX];
    memset(rowEmpty, true, sizeof(rowEmpty));
    memset(colEmpty, true, sizeof(colEmpty));

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (grid[i][j] == '#') {
                rowEmpty[i] = false;
                colEmpty[j] = false;
            }
        }
    }

    vector<int> emptyRows;
    vector<int> emptyCols;
    for (int i = 0; i < R; i++) {
        if (rowEmpty[i]) {
            emptyRows.push_back(i);
        }
    }
    for (int j = 0; j < C; j++) {
        if (colEmpty[j]) {
            emptyCols.push_back(j);
        }
    }

    int A = emptyRows.size();
    int B = emptyCols.size();

    vector<int> graph[MAX];
    for (int i = 0; i < A; i++) {
        int r = emptyRows[i];
        for (int j = 0; j < B; j++) {
            int c = emptyCols[j];
            if (grid[r][c] == '.') {
                graph[i].push_back(j);
            }
        }
    }

    int matchR[MAX];
    memset(matchR, -1, sizeof(matchR));
    int matchingCount = 0;
    bool seen[MAX];

    for (int u = 0; u < A; u++) {
        memset(seen, 0, sizeof(seen));
        if (dfs(u, seen, graph, matchR)) {
            matchingCount++;
        }
    }

    int answer = A + B - matchingCount;
    cout << answer << endl;

    return 0;
}