#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int64 MOD = 998244353;

int64 modpow(int64 a, long long e) {
    int64 r = 1 % MOD;
    a %= MOD;
    while (e > 0) {
        if (e & 1) r = (r * a) % MOD;
        a = (a * a) % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long r, c;
    if (!(cin >> r >> c)) return 0;

    long long R = r / 6;
    long long C = c / 7;
    long long M = R * C;

    // Precompute factorials and inverse factorials up to 42
    const int N = 42;
    vector<int64> fact(N + 1), invfact(N + 1);
    fact[0] = 1;
    for (int i = 1; i <= N; ++i) fact[i] = fact[i - 1] * i % MOD;
    invfact[N] = modpow(fact[N], MOD - 2);
    for (int i = N; i >= 1; --i) invfact[i - 1] = invfact[i] * i % MOD;

    auto Cnk = [&](int n, int k) -> int64 {
        if (k < 0 || k > n) return 0;
        return fact[n] * invfact[k] % MOD * invfact[n - k] % MOD;
    };

    int64 ans = 0;
    for (int S = 0; S <= 42; ++S) {
        int64 comb = Cnk(42, S);
        ans += modpow(comb, M);
        if (ans >= MOD) ans -= MOD;
    }

    cout << ans << '\n';
    return 0;
}