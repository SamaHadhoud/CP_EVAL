#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 998244353;

long long mod_pow(long long base, long long exp) {
    long long res = 1 % MOD;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());
    long long s = (long long)a.size();
    
    long long E = (s - 1) * (long long)n - (s * (s - 1)) / 2; // rank
    long long ans = mod_pow(2, E);
    cout << ans << '\n';
    return 0;
}