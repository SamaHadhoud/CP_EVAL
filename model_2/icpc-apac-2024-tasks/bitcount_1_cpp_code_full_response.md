```cpp
#include <bits/stdc++.h>
using namespace std;

using u128 = unsigned __int128;
using u64 = unsigned long long;

// Convert unsigned __int128 to string
string to_string_u128(u128 x) {
    if (x == 0) return "0";
    string s;
    while (x > 0) {
        int digit = (int)(x % 10);
        s.push_back('0' + digit);
        x /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        if (n == 1) {
            int A = (int)a[0];
            if (A < 0) { cout << -1 << '\n'; continue; }
            u128 x = (u128(1) << A) - 1; // smallest x with popcount A
            cout << to_string_u128(x) << '\n';
            continue;
        }

        int m = n - 1;
        vector<int> b(m);
        bool ok = true;
        int max_b = 0;
        for (int i = 0; i < m; ++i) {
            long long d = a[i+1] - a[i];
            long long bi = 1 - d; // v2(x+i+1)
            if (bi < 0) { ok = false; break; }
            if (bi > (long long)INT_MAX) { ok = false; break; }
            b[i] = (int)bi;
            if (b[i] > max_b) max_b = b[i];
        }
        if (!ok) { cout << -1 << '\n'; continue; }

        int B = 1 + max_b;
        // m < 2^B is necessary
        if (B >= 64) {
            // since m <= 5e5 and 2^64 is huge, this condition always true;
            // but per constraints b_i <= 61 so B <= 62. This is just a guard.
        }
        u64 pow2B_u64 = (B >= 64 ? 0ULL : (1ULL << B)); // if B>=64 not used for comparison
        if (B < 64 && (u64)m >= pow2B_u64) {
            cout << -1 << '\n';
            continue;
        }

        // Reconstruct r modulo 2^B
        u64 r_mod = 0; // r modulo current modulus
        u64 mod = 1;   // current modulus = 2^(k-1) in loop, starts at 1 for k=1

        for (int k = 1; k <= B - 1 && ok; ++k) {
            u64 pow2k = (1ULL << k);
            u64 mask = pow2k - 1;

            bool found = false;
            u64 s = 0;
            for (int i = 0; i < m; ++i) {
                if (b[i] >= k) {
                    // v = (- (i+1)) mod 2^k
                    u64 v = (u64)(-(u64)(i + 1)) & mask;
                    if (!found) { s = v; found = true; }
                    else if (v != s) { ok = false; break; }
                }
            }
            if (!ok) break;
            if (!found) { ok = false; break; } // should not happen since k <= max_b

            // Consistency with previous level
            if (k > 1) {
                u64 prev_mask = (1ULL << (k - 1)) - 1;
                if ((s & prev_mask) != r_mod) { ok = false; break; }
            }
            r_mod = s;
            mod = pow2k;

            // Fullness check: every index i ≡ t (mod 2^k) must have b[i] >= k
            u64 tclass = (u64)(-(r_mod + 1)) & mask; // (-r-1) mod 2^k
            for (u64 i = tclass; i < (u64)m; i += pow2k) {
                if (b[(size_t)i] < k) { ok = false; break; }
            }
        }
        if (!ok) { cout << -1 << '\n'; continue; }

        // Decide top bit (k = B)
        u64 pow2B = (1ULL << B);
        u64 maskB = pow2B - 1;

        bool seen_forbid = false;
        u64 vforbid = 0;
        for (int i = 0; i < m; ++i) {
            if (b[i] == B - 1) {
                u64 v = (u64)(-(u64)(i + 1)) & maskB; // (- (i+1)) mod 2^B
                if (!seen_forbid) { vforbid = v; seen_forbid = true; }
                else if (v != vforbid) { ok = false; break; }
            }
        }
        if (!ok || !seen_forbid) { cout << -1 << '\n'; continue; }

        // Lift r from mod 2^(B-1) to mod 2^B
        u64 lift0 = r_mod;           // bit (B-1) = 0
        u64 lift1 = r_mod + (1ULL << (B - 1)); // bit (B-1) = 1

        if (vforbid == lift0) r_mod = lift1;
        else if (vforbid == lift1) r_mod = lift0;
        else { cout << -1 << '\n'; continue; }

        // Now r_mod is x modulo 2^B.
        int pr = __builtin_popcountll(r_mod);
        long long delta = a[0] - pr;
        if (delta < 0) { cout << -1 << '\n'; continue; }

        u128 tmin = (u128(1) << (unsigned)delta) - 1; // minimal t with popcount = delta
        u128 x = (u128)r_mod + (u128(1) << B) * tmin;

        cout << to_string_u128(x) << '\n';
    }
    return 0;
}
```