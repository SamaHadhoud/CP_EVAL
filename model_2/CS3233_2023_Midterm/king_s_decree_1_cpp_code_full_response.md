```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int C;
    if (!(cin >> C)) return 0;
    while (C--) {
        int n;
        cin >> n;
        long long W = 0;
        for (int i = 0; i < n; ++i) {
            long long w;
            cin >> w;
            W += w;
        }
        vector<long long> L(n);
        long long sumL = 0;
        for (int i = 0; i < n; ++i) {
            cin >> L[i];
            sumL += L[i];
        }
        sort(L.begin(), L.end());
        long long E = W - sumL; // extra wealth available above floors
        
        long long ans = 0;
        bool done = false;
        for (int i = 0; i < n - 1; ++i) {
            long long k = i + 1LL;               // number of columns currently being raised
            long long gap = L[i + 1] - L[i];     // next floor to reach
            if (gap == 0) continue;
            long long need = k * gap;
            if (E >= need) {
                E -= need; // we can reach the next distinct floor
            } else {
                ans = L[i] + E / k; // can't reach next floor; distribute remaining evenly
                done = true;
                break;
            }
        }
        if (!done) {
            ans = L[n - 1] + E / n; // raise all equally with remaining extra
        }
        cout << ans << '\n';
    }
    return 0;
}
```