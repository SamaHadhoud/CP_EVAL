Below is an optimized C++17 solution that precomputes all n ≤ 1e18 with non-decreasing digits such that n^2 also has non-decreasing digits. It uses a right-to-left DFS with suffix-based pruning (as described), then answers each query by binary searching the precomputed list.

```cpp
#include <bits/stdc++.h>
using namespace std;

static const unsigned long long LIMIT = 1000000000000000000ULL; // 1e18
unsigned long long pow10arr[19];
vector<unsigned long long> good;

inline void dfs(int lastDigit, unsigned long long sVal, int L) {
    __int128 T = (__int128)sVal * sVal;

    // Suffix (last L digits) check for non-increasing digits from LSD to MSD
    int prev = 10;
    for (int i = 0; i < L; ++i) {
        int cur = (int)(T % 10);
        if (cur > prev) return; // prune
        prev = cur;
        T /= 10;
    }

    // Full square check for the current complete number sVal
    bool ok = true;
    while (T > 0) {
        int cur = (int)(T % 10);
        if (cur > prev) { ok = false; break; }
        prev = cur;
        T /= 10;
    }
    if (ok) good.push_back(sVal);

    // Try to extend to the left by adding a new digit d in [1..lastDigit]
    if (L == 19) return; // no need to go further (safety guard)
    for (int d = 1; d <= lastDigit; ++d) {
        __int128 newV = (__int128)d * pow10arr[L] + sVal;
        if (newV > LIMIT) break; // increasing in d, so we can break
        dfs(d, (unsigned long long)newV, L + 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Precompute powers of 10 up to 10^18
    pow10arr[0] = 1;
    for (int i = 1; i <= 18; ++i) pow10arr[i] = pow10arr[i - 1] * 10ULL;

    // Precompute all "good" numbers once
    for (int d = 1; d <= 9; ++d) dfs(d, (unsigned long long)d, 1);
    sort(good.begin(), good.end());

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        unsigned long long X;
        cin >> X;
        cout << (upper_bound(good.begin(), good.end(), X) - good.begin()) << '\n';
    }
    return 0;
}
```