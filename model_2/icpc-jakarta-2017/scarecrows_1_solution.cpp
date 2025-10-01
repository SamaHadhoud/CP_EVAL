#include <bits/stdc++.h>
using namespace std;

static const int MOD = 1'000'000'007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int R, C;
    if (!(cin >> R >> C)) return 0;
    
    vector<int> A(C, 0); // column masks
    bool impossible = false;
    for (int r = 0; r < R; ++r) {
        string s; cin >> s;
        bool hasDot = false;
        for (int j = 0; j < C; ++j) {
            if (s[j] == '.') {
                hasDot = true;
                A[j] |= (1 << r);
            }
        }
        if (!hasDot) impossible = true;
    }
    if (impossible) {
        cout << 0 << '\n';
        return 0;
    }
    
    int fullMask = (1 << R) - 1;
    
    // Precompute popcount for all masks up to 2^R - 1
    vector<int> pc(1 << R, 0);
    for (int m = 1; m <= fullMask; ++m) {
        pc[m] = pc[m >> 1] + (m & 1);
    }
    
    // Precompute powers of 2 up to R
    vector<int> pow2(R + 1, 1);
    for (int i = 1; i <= R; ++i) {
        pow2[i] = (pow2[i - 1] * 2LL) % MOD;
    }
    
    long long ans = 0;
    for (int T = 0; T <= fullMask; ++T) {
        int allowed = fullMask ^ T;
        
        // DP over columns with two states: dp0 - current column empty, dp1 - current column non-empty
        int k0 = pc[A[0] & allowed];
        int w0 = pow2[k0] - 1; if (w0 < 0) w0 += MOD;
        int dp0 = 1;       // first column empty
        int dp1 = w0;      // first column non-empty
        
        for (int j = 1; j < C; ++j) {
            int kj = pc[A[j] & allowed];
            int wj = pow2[kj] - 1; if (wj < 0) wj += MOD;
            int ndp0 = dp1;
            int ndp1 = (int)(( (dp0 + dp1) % MOD ) * 1LL * wj % MOD);
            dp0 = ndp0;
            dp1 = ndp1;
        }
        
        int g = dp0; g += dp1; if (g >= MOD) g -= MOD;
        if ((pc[T] & 1) == 0) {
            ans += g;
            if (ans >= MOD) ans -= MOD;
        } else {
            ans -= g;
            if (ans < 0) ans += MOD;
        }
    }
    
    cout << ans % MOD << '\n';
    return 0;
}