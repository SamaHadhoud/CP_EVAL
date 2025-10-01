#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int R, C;
    if (!(cin >> R >> C)) return 0;
    vector<string> g(R);
    for (int i = 0; i < R; ++i) cin >> g[i];

    long long ans = 0;

    if (R >= 3 && C >= 3) {
        for (int r = 1; r <= R - 2; ++r)
            for (int c = 1; c <= C - 2; ++c)
                if (g[r][c] == '.') ++ans;
    } else if (R == 1) {
        for (int c = 1; c <= C - 2; ++c)
            if (g[0][c] == '.') ++ans;
    } else if (C == 1) {
        for (int r = 1; r <= R - 2; ++r)
            if (g[r][0] == '.') ++ans;
    } else if (R == 2) {
        for (int c = 1; c <= C - 2; ++c)
            if (g[0][c] == '.' && g[1][c] == '.') ++ans;
    } else { // C == 2
        for (int r = 1; r <= R - 2; ++r)
            if (g[r][0] == '.' && g[r][1] == '.') ++ans;
    }

    cout << ans << '\n';
    return 0;
}