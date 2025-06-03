#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
using namespace std;

typedef pair<int, int> pii;
typedef long long ll;

ll gcd(ll a, ll b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int R, C;
        cin >> R >> C;
        vector<string> grid(R);
        for (int i = 0; i < R; i++) {
            cin >> grid[i];
        }

        int r0 = 0;
        while (r0 < R) {
            if (grid[r0].find('#') != string::npos) break;
            r0++;
        }
        if (r0 == R) {
            cout << "0/1\n";
            continue;
        }

        int r1 = R - 1;
        while (r1 >= 0 && grid[r1].find('#') == string::npos) {
            r1--;
        }
        vector<int> cols_with_hash;
        for (int j = 0; j < C; j++) {
            for (int i = 0; i < R; i++) {
                if (grid[i][j] == '#') {
                    cols_with_hash.push_back(j);
                    break;
                }
            }
        }
        if (cols_with_hash.empty()) {
            cout << "0/1\n";
            continue;
        }
        int c0 = *min_element(cols_with_hash.begin(), cols_with_hash.end());
        int c1 = *max_element(cols_with_hash.begin(), cols_with_hash.end());

        int H = r1 - r0 + 1;
        int W = c1 - c0 + 1;
        vector<string> grid0;
        for (int i = r0; i <= r1; i++) {
            grid0.push_back(grid[i].substr(c0, W));
        }

        set<pii> S0;
        set<pii> border0;
        const vector<pii> directions = {{-1,-1}, {-1,0}, {-1,1},
                          {0,-1},           {0,1},
                          {1,-1},  {1,0},  {1,1}};

        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (grid0[i][j] == '#') {
                    S0.insert({i, j});
                    bool is_border = false;
                    for (const auto& dir : directions) {
                        int ni = i + dir.first;
                        int nj = j + dir.second;
                        if (ni < 0 || ni >= H || nj < 0 || nj >= W) {
                            is_border = true;
                            break;
                        }
                        if (grid0[ni][nj] != '#') {
                            is_border = true;
                            break;
                        }
                    }
                    if (is_border) {
                        border0.insert({i, j});
                    }
                }
            }
        }

        vector<int> rotations = {0, 90, 180, 270};
        ll best_num = 0;
        ll best_den = 1;

        for (int rot : rotations) {
            set<pii> S_rot;
            set<pii> border_rot;

            if (rot == 0) {
                S_rot = S0;
                border_rot = border0;
            } else if (rot == 90) {
                for (const auto& p : S0) {
                    int i = p.first, j = p.second;
                    S_rot.insert({j, H-1-i});
                }
                for (const auto& p : border0) {
                    int i = p.first, j = p.second;
                    border_rot.insert({j, H-1-i});
                }
            } else if (rot == 180) {
                for (const auto& p : S0) {
                    int i = p.first, j = p.second;
                    S_rot.insert({H-1-i, W-1-j});
                }
                for (const auto& p : border0) {
                    int i = p.first, j = p.second;
                    border_rot.insert({H-1-i, W-1-j});
                }
            } else if (rot == 270) {
                for (const auto& p : S0) {
                    int i = p.first, j = p.second;
                    S_rot.insert({W-1-j, i});
                }
                for (const auto& p : border0) {
                    int i = p.first, j = p.second;
                    border_rot.insert({W-1-j, i});
                }
            }

            map<pii, ll> A_map;
            map<pii, ll> B_map;

            for (const auto& p : border0) {
                for (const auto& q : S_rot) {
                    int dx = p.first - q.first;
                    int dy = p.second - q.second;
                    A_map[{dx, dy}]++;
                }
            }

            for (const auto& q : border_rot) {
                for (const auto& p : S0) {
                    int dx = p.first - q.first;
                    int dy = p.second - q.second;
                    B_map[{dx, dy}]++;
                }
            }

            for (const auto& kv : B_map) {
                pii key = kv.first;
                ll B_val = kv.second;
                if (B_val == 0) continue;
                if (A_map.find(key) != A_map.end()) {
                    ll A_val = A_map[key];
                    if (A_val == 0) continue;

                    if (A_val * best_den > best_num * B_val) {
                        best_num = A_val;
                        best_den = B_val;
                    }
                }
            }
        }

        if (best_den == 0) {
            cout << "0/1\n";
        } else {
            ll g = gcd(best_num, best_den);
            best_num /= g;
            best_den /= g;
            cout << best_num << '/' << best_den << '\n';
        }
    }

    return 0;
}