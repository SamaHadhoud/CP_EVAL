#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long K;
    string S;
    if (!(cin >> K)) return 0;
    cin >> S;
    int n = (int)S.size();

    // L0 = floor(log2 K) + 1
    int L0 = 0;
    {
        unsigned long long t = K;
        while (t) { ++L0; t >>= 1; }
    }

    // If n <= L0 - 1, whole S is <= K
    if (n <= L0 - 1) {
        cout << 0 << '\n';
        return 0;
    }

    // Build Kbits as L0-bit representation of K (MSB first)
    vector<int> Kbits(L0);
    {
        unsigned long long t = K;
        for (int i = L0 - 1; i >= 0; --i) {
            Kbits[i] = int(t & 1ULL);
            t >>= 1ULL;
        }
    }

    // DP over subsequences to check feasibility of length L0
    vector<array<char, 2>> dp(L0 + 1), newdp(L0 + 1);
    for (auto &a : dp) a = {0, 0};
    dp[0][0] = 1; // chosen 0 bits, equal to K prefix

    for (int i = 0; i < n; ++i) {
        newdp = dp; // skipping S[i]
        int b = S[i] - '0';
        for (int j = 0; j <= L0 - 1; ++j) {
            for (int less = 0; less <= 1; ++less) {
                if (!dp[j][less]) continue;
                if (j == 0 && b == 0) continue; // no leading zero
                if (less) {
                    newdp[j + 1][1] = 1; // already smaller, can take any bit
                } else {
                    int kb = Kbits[j];
                    if (b < kb) newdp[j + 1][1] = 1;
                    else if (b == kb) newdp[j + 1][0] = 1;
                    // if b > kb: cannot take
                }
            }
        }
        dp.swap(newdp);
    }

    bool feasible = dp[L0][0] || dp[L0][1];
    int ans = feasible ? (n - L0) : (n - (L0 - 1));
    cout << ans << '\n';
    return 0;
}