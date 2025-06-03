#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
using namespace std;

const int MAXM = 3000;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<string> grid;
    grid.reserve(n);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        grid.push_back(s);
    }

    vector<vector<int>> non_dot_positions(n);
    vector<bitset<MAXM>> student_bits(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] != '.') {
                non_dot_positions[i].push_back(j);
                student_bits[i].set(j);
            }
        }
    }

    for (int b = 1; b < n; b++) {
        for (int a = b - 1; a >= 0; a--) {
            int da = non_dot_positions[a].size();
            int db = non_dot_positions[b].size();
            if (min(da, db) < k) {
                continue;
            }

            bitset<MAXM> common = student_bits[a] & student_bits[b];
            if (common.count() < static_cast<size_t>(k)) {
                continue;
            }

            int count = 0;
            if (da <= db) {
                for (int idx = 0; idx < da; idx++) {
                    int j = non_dot_positions[a][idx];
                    if (common[j] && grid[a][j] == grid[b][j]) {
                        count++;
                        if (count >= k) {
                            break;
                        }
                    }
                }
            } else {
                for (int idx = 0; idx < db; idx++) {
                    int j = non_dot_positions[b][idx];
                    if (common[j] && grid[a][j] == grid[b][j]) {
                        count++;
                        if (count >= k) {
                            break;
                        }
                    }
                }
            }

            if (count >= k) {
                cout << a + 1 << " " << b + 1 << endl;
                return 0;
            }
        }
    }

    cout << -1 << endl;
    return 0;
}