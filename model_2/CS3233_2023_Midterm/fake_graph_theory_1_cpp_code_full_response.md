```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if (!(cin >> n)) return 0;
    
    const long long MOD = 1000003233LL;
    auto mul = [&](long long a, long long b) -> long long {
        return (long long)((__int128)a * b % MOD);
    };
    
    long long F = 1;       // F_k = ((2k)!)^2 mod MOD, start with F_0 = 1
    long long Eprime = 0;  // E'_k target accumulator, start with 0
    
    for (long long k = 1; k <= n; ++k) {
        long long a = (2 * k) % MOD;        // 2k
        long long b = (2 * k - 1) % MOD;    // 2k - 1
        long long a2 = mul(a, a);           // (2k)^2
        long long b2 = mul(b, b);           // (2k - 1)^2
        
        // E'_k = (2k)^2 (2k-1)^2 E'_{k-1} + (2k)^2 (2k-1) F_{k-1}
        long long t1 = mul(mul(Eprime, a2), b2);
        long long t2 = mul(mul(F, a2), b);
        Eprime = t1 + t2;
        if (Eprime >= MOD) Eprime -= MOD;
        
        // F_k = F_{k-1} * (2k)^2 * (2k - 1)^2
        F = mul(mul(F, a2), b2);
    }
    
    cout << Eprime % MOD << '\n';
    return 0;
}
```