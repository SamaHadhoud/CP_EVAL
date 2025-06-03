#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

typedef unsigned long long ull;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C, Q;
    cin >> R >> C >> Q;
    vector<string> grid(R);
    for (int i = 0; i < R; i++) {
        cin >> grid[i];
    }

    int max_len = max(R, C);
    vector<ull> pow_base(max_len + 1);
    pow_base[0] = 1;
    for (int i = 1; i <= max_len; i++) {
        pow_base[i] = pow_base[i - 1] * 131;
    }

    vector<vector<ull>> H_row(R, vector<ull>(C + 1, 0));
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            H_row[r][c + 1] = H_row[r][c] * 131 + (grid[r][c] - 'A' + 1);
        }
    }

    vector<vector<ull>> H_col(C, vector<ull>(R + 1, 0));
    for (int c = 0; c < C; c++) {
        for (int r = 0; r < R; r++) {
            H_col[c][r + 1] = H_col[c][r] * 131 + (grid[r][c] - 'A' + 1);
        }
    }

    vector<ull> freq(26, 0);
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            freq[grid[r][c] - 'A']++;
        }
    }

    vector<vector<int>> hor_pairs(26, vector<int>(26, 0));
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C - 1; c++) {
            int a = grid[r][c] - 'A';
            int b = grid[r][c + 1] - 'A';
            hor_pairs[a][b]++;
        }
    }

    vector<vector<int>> ver_pairs(26, vector<int>(26, 0));
    for (int r = 0; r < R - 1; r++) {
        for (int c = 0; c < C; c++) {
            int a = grid[r][c] - 'A';
            int b = grid[r + 1][c] - 'A';
            ver_pairs[a][b]++;
        }
    }

    while (Q--) {
        string S;
        cin >> S;
        int L = S.size();

        if (L == 1) {
            cout << freq[S[0] - 'A'] << '\n';
        } else if (L == 2) {
            int a = S[0] - 'A';
            int b = S[1] - 'A';
            cout << hor_pairs[a][b] + ver_pairs[a][b] << '\n';
        } else {
            if (L > R + C - 1) {
                cout << 0 << '\n';
                continue;
            }

            vector<ull> H_S(L + 1, 0);
            for (int j = 0; j < L; j++) {
                H_S[j + 1] = H_S[j] * 131 + (S[j] - 'A' + 1);
            }

            int i0 = max(0, L - R);
            int i1 = min(L - 1, C - 1);
            if (i0 > i1) {
                cout << 0 << '\n';
                continue;
            }

            ull ans = 0;
            for (int i = i0; i <= i1; i++) {
                int h_len = i + 1;
                int v_len = L - i - 1;

                ull hash_horiz = H_S[h_len];
                ull hash_vert = 0;
                if (v_len > 0) {
                    hash_vert = H_S[i + 1 + v_len] - H_S[i + 1] * pow_base[v_len];
                }

                int max_r = R - (L - i);
                int max_c = C - h_len;
                if (max_r < 0 || max_c < 0) {
                    continue;
                }

                for (int r = 0; r <= max_r; r++) {
                    for (int c = 0; c <= max_c; c++) {
                        ull h_row_val = H_row[r][c + h_len] - H_row[r][c] * pow_base[h_len];
                        if (h_row_val != hash_horiz) {
                            continue;
                        }

                        if (v_len > 0) {
                            ull h_col_val = H_col[c + i][r + 1 + v_len] - H_col[c + i][r + 1] * pow_base[v_len];
                            if (h_col_val != hash_vert) {
                                continue;
                            }
                        }
                        ans++;
                    }
                }
            }
            cout << ans << '\n';
        }
    }

    return 0;
}