#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cmath>
using namespace std;

const long long MOD = 998244353;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    long long total_xor = 0;
    for (long long x : a) {
        total_xor ^= x;
    }

    bool all_same = true;
    for (int i = 1; i < n; i++) {
        if (a[i] != a[0]) {
            all_same = false;
            break;
        }
    }

    if (all_same) {
        cout << 1 << endl;
        return 0;
    }

    int rank = 0;
    vector<long long> basis;
    for (int i = 0; i < n; i++) {
        long long x = a[i];
        for (long long b : basis) {
            if ((x ^ b) < x) 
                x ^= b;
        }
        if (x != 0) {
            basis.push_back(x);
            sort(basis.rbegin(), basis.rend());
            rank++;
        }
    }

    long long power = (long long)rank * (n - 1);
    long long ans = 1;
    long long base = 2;
    while (power > 0) {
        if (power % 2 == 1) {
            ans = (ans * base) % MOD;
        }
        base = (base * base) % MOD;
        power /= 2;
    }

    cout << ans << endl;

    return 0;
}