```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;

    const long long MOD = 1000003233LL;

    // Compute C(N, k) mod MOD via Pascal's triangle row DP (O(N^2))
    vector<long long> C(N + 1, 0);
    C[0] = 1;
    for (int n = 1; n <= N; ++n) {
        for (int k = n; k >= 1; --k) {
            C[k] += C[k - 1];
            if (C[k] >= MOD) C[k] -= MOD;
        }
    }

    auto mul = [&](long long a, long long b) -> long long {
        return (long long)((__int128)a * b % MOD);
    };

    auto mod_pow = [&](long long a, long long e) -> long long {
        long long r = 1 % MOD, x = a % MOD;
        while (e > 0) {
            if (e & 1) r = mul(r, x);
            x = mul(x, x);
            e >>= 1;
        }
        return r;
    };

    long long pow2 = mod_pow(2, 1LL * N * N);
    long long inv2 = (MOD + 1) / 2; // since MOD is odd
    long long t = pow2;
    long long ans = 0;

    for (int k = 0; k <= N; ++k) {
        long long term = mul(C[k], C[k]);     // C(N,k)^2 mod MOD
        ans += mul(term, t);                   // * 2^{N^2 - k}
        if (ans >= MOD) ans -= MOD;
        t = mul(t, inv2);                      // divide by 2 modulo MOD
    }

    cout << ans << '\n';
    return 0;
}
```