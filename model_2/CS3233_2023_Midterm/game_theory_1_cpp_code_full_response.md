#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> p(n + 1);
        for (int i = 1; i <= n; ++i) cin >> p[i];
        vector<long long> a(n + 1);
        for (int i = 1; i <= n; ++i) cin >> a[i];

        vector<long long> dpMax(n + 2, 0), dpMin(n + 2, 0);

        for (int x = n; x >= 1; --x) {
            bool have = false;
            long long bestMax = LLONG_MIN; // max over dpMin[y]
            long long bestMin = LLONG_MAX; // min over dpMax[y]
            for (int y = x + p[x]; y <= n; y += p[x]) {
                have = true;
                if (dpMin[y] > bestMax) bestMax = dpMin[y];
                if (dpMax[y] < bestMin) bestMin = dpMax[y];
            }
            long long moveUp = have ? max(0LL, bestMax) : 0LL;
            long long moveDown = have ? min(0LL, bestMin) : 0LL;
            dpMax[x] = a[x] + moveUp;
            dpMin[x] = a[x] + moveDown;
        }

        for (int i = 1; i <= n; ++i) {
            if (i > 1) cout << ' ';
            cout << dpMax[i];
        }
        cout << '\n';
    }
    return 0;
}