```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<int>> a(n, vector<int>(n));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                cin >> a[i][j];

        vector<int> badRows, badCols;
        vector<char> isBadRow(n, false), isBadCol(n, false);

        // Identify bad rows (all values distinct -> permutation)
        for (int i = 0; i < n; ++i) {
            vector<char> seen(n + 1, 0);
            bool bad = true;
            for (int j = 0; j < n; ++j) {
                int v = a[i][j];
                if (seen[v]) { bad = false; } // has duplicate -> not bad
                else seen[v] = 1;
            }
            if (bad) {
                isBadRow[i] = 1;
                badRows.push_back(i);
            }
        }

        // Identify bad columns (all values distinct -> permutation)
        for (int j = 0; j < n; ++j) {
            vector<char> seen(n + 1, 0);
            bool bad = true;
            for (int i = 0; i < n; ++i) {
                int v = a[i][j];
                if (seen[v]) { bad = false; } // has duplicate -> not bad
                else seen[v] = 1;
            }
            if (bad) {
                isBadCol[j] = 1;
                badCols.push_back(j);
            }
        }

        vector<tuple<int,int,int>> ops;
        auto apply = [&](int i, int j, int v) {
            a[i][j] = v;
            ops.emplace_back(i + 1, j + 1, v);
        };

        if (badRows.empty() && badCols.empty()) {
            cout << 0 << '\n';
            continue;
        }

        if (badRows.size() >= badCols.size()) {
            int c = (int)badCols.size();

            // Pair bad rows with bad columns
            for (int k = 0; k < c; ++k) {
                int i = badRows[k];
                int j = badCols[k];
                int v = a[i][j] % n + 1; // any value != a[i][j], still in [1..n]
                if (v == a[i][j]) v = (a[i][j] == n ? 1 : a[i][j] + 1);
                apply(i, j, v);
            }

            // Fix remaining bad rows using good columns
            int rLeft = (int)badRows.size() - c;
            vector<int> goodCols;
            goodCols.reserve(n - c);
            vector<char> inBadCol(n, 0);
            for (int j : badCols) inBadCol[j] = 1;
            for (int j = 0; j < n; ++j) if (!inBadCol[j]) goodCols.push_back(j);

            for (int idx = 0; idx < rLeft; ++idx) {
                int i = badRows[c + idx];
                int j = goodCols[idx];

                int base = a[i][j];
                int v = base;
                bool foundDiffInCol = false;
                for (int s = 0; s < n; ++s) {
                    if (s != i && a[s][j] != base) {
                        v = a[s][j]; // duplicate value v in column j
                        foundDiffInCol = true;
                        break;
                    }
                }
                if (foundDiffInCol) {
                    // Row i is a permutation, so v appears elsewhere in row i
                    apply(i, j, v);
                } else {
                    // Column j is constant: choose any other value from row i (exists since row i is permutation)
                    int pAlt = -1;
                    for (int p = 0; p < n; ++p) if (p != j && a[i][p] != base) { pAlt = p; break; }
                    int vv = (pAlt == -1 ? base : a[i][pAlt]); // pAlt must exist for permutation row
                    if (vv == base) { // safety fallback, though shouldn't happen
                        vv = base % n + 1;
                        if (vv == base) vv = (base == n ? 1 : base + 1);
                    }
                    apply(i, j, vv);
                }
            }

        } else {
            int r = (int)badRows.size();

            // Pair bad rows with bad columns
            for (int k = 0; k < r; ++k) {
                int i = badRows[k];
                int j = badCols[k];
                int v = a[i][j] % n + 1; // any value != a[i][j], still in [1..n]
                if (v == a[i][j]) v = (a[i][j] == n ? 1 : a[i][j] + 1);
                apply(i, j, v);
            }

            // Fix remaining bad columns using good rows
            int cLeft = (int)badCols.size() - r;
            vector<int> goodRows;
            goodRows.reserve(n - r);
            vector<char> inBadRow(n, 0);
            for (int i : badRows) inBadRow[i] = 1;
            for (int i = 0; i < n; ++i) if (!inBadRow[i]) goodRows.push_back(i);

            for (int idx = 0; idx < cLeft; ++idx) {
                int j = badCols[r + idx];
                int i = goodRows[idx];

                int base = a[i][j];

                // Check if row i is constant
                bool rowConst = true;
                for (int p = 1; p < n; ++p) {
                    if (a[i][p] != a[i][0]) { rowConst = false; break; }
                }

                if (!rowConst) {
                    // Pick any column p != j with a different value; copy it to (i, j)
                    int pPick = -1;
                    for (int p = 0; p < n; ++p) {
                        if (p != j && a[i][p] != base) { pPick = p; break; }
                    }
                    // pPick must exist since row is not constant
                    int v = a[i][pPick];
                    apply(i, j, v); // duplicates in row i for value v; column j: v != base so it duplicates v in that column
                } else {
                    // Row is constant: change to any different value present elsewhere in column j
                    int sPick = -1;
                    for (int s = 0; s < n; ++s) {
                        if (s != i && a[s][j] != base) { sPick = s; break; }
                    }
                    // Since column j is a permutation (bad column), sPick must exist
                    int v = a[sPick][j];
                    apply(i, j, v); // row i stays good (n-1 copies of base remain), column j gets duplicate v
                }
            }
        }

        cout << ops.size() << '\n';
        for (auto &e : ops) {
            int i, j, v;
            tie(i, j, v) = e;
            cout << i << ' ' << j << ' ' << v << '\n';
        }
    }
    return 0;
}
```