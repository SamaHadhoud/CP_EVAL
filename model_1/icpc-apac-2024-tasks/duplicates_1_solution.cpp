#include <iostream>
#include <vector>
#include <set>
#include <tuple>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<int>> mat(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> mat[i][j];
            }
        }

        vector<int> bad_rows;
        for (int i = 0; i < n; i++) {
            set<int> row_set;
            for (int j = 0; j < n; j++) {
                row_set.insert(mat[i][j]);
            }
            if (row_set.size() == n) {
                bad_rows.push_back(i);
            }
        }

        vector<int> bad_cols;
        for (int j = 0; j < n; j++) {
            set<int> col_set;
            for (int i = 0; i < n; i++) {
                col_set.insert(mat[i][j]);
            }
            if (col_set.size() == n) {
                bad_cols.push_back(j);
            }
        }

        int r = bad_rows.size();
        int c = bad_cols.size();
        int k = min(r, c);
        vector<tuple<int, int, int>> mods;

        for (int i = 0; i < k; i++) {
            int i_row = bad_rows[i];
            int j_col = bad_cols[i];
            int a = mat[i_row][j_col];
            int v;
            if (a != 1) {
                v = 1;
            } else {
                v = 2;
            }
            mods.push_back(make_tuple(i_row, j_col, v));
        }

        for (int i = k; i < r; i++) {
            int i_row = bad_rows[i];
            int j_col = 0;
            int a = mat[i_row][j_col];
            set<int> col_set;
            for (int ii = 0; ii < n; ii++) {
                col_set.insert(mat[ii][j_col]);
            }
            if (col_set.find(a) != col_set.end()) {
                col_set.erase(a);
            }
            int v;
            if (!col_set.empty()) {
                v = *(col_set.begin());
            } else {
                if (a != 1) {
                    v = 1;
                } else {
                    v = 2;
                }
            }
            mods.push_back(make_tuple(i_row, j_col, v));
        }

        for (int j = k; j < c; j++) {
            int j_col = bad_cols[j];
            int i_row = 0;
            int a = mat[i_row][j_col];
            set<int> row_set;
            for (int jj = 0; jj < n; jj++) {
                row_set.insert(mat[i_row][jj]);
            }
            if (row_set.find(a) != row_set.end()) {
                row_set.erase(a);
            }
            int v;
            if (!row_set.empty()) {
                v = *(row_set.begin());
            } else {
                if (a != 1) {
                    v = 1;
                } else {
                    v = 2;
                }
            }
            mods.push_back(make_tuple(i_row, j_col, v));
        }

        cout << mods.size() << "\n";
        for (auto &mod : mods) {
            int i, j, v;
            tie(i, j, v) = mod;
            cout << i+1 << " " << j+1 << " " << v << "\n";
        }
    }
    return 0;
}