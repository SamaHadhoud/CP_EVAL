#include <iostream>
#include <vector>
using namespace std;

typedef long long ll;

const ll MOD = 1000003233;
const int MAX_N = 3233;
const int MAX_FACT = 2 * MAX_N + 10;

ll mod_pow(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) 
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    int n;
    cin >> n;

    vector<ll> F(MAX_FACT + 1, 1);
    vector<ll> invF(MAX_FACT + 1, 1);
    for (int i = 1; i <= MAX_FACT; i++) {
        F[i] = F[i - 1] * i % MOD;
    }
    invF[MAX_FACT] = mod_pow(F[MAX_FACT], MOD - 2, MOD);
    for (int i = MAX_FACT - 1; i >= 0; i--) {
        invF[i] = invF[i + 1] * (i + 1) % MOD;
    }

    vector<ll> pow4(MAX_N + 1, 1);
    for (int i = 1; i <= MAX_N; i++) {
        pow4[i] = pow4[i - 1] * 4 % MOD;
    }

    vector<ll> inv_k(MAX_N + 1, 1);
    for (int k = 1; k <= MAX_N; k++) {
        inv_k[k] = mod_pow(k, MOD - 2, MOD);
    }

    ll inv2 = mod_pow(2, MOD - 2, MOD);

    ll total_sum = 0;
    for (int k = 1; k <= n; k++) {
        int index_2n_minus_2k = 2 * n - 2 * k;
        if (index_2n_minus_2k < 0) break;
        ll term = F[index_2n_minus_2k] * F[index_2n_minus_2k] % MOD;
        term = term * pow4[k] % MOD;
        term = term * inv_k[k] % MOD;
        term = term * invF[2 * n - k] % MOD;
        term = term * invF[n - k] % MOD;
        term = term * invF[n - k] % MOD;
        total_sum = (total_sum + term) % MOD;
    }

    ll result = F[2 * n] * F[n] % MOD;
    result = result * F[n] % MOD;
    result = result * total_sum % MOD;
    result = result * inv2 % MOD;

    cout << result << endl;

    return 0;
}