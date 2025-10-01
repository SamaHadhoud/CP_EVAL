#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long L_in, R_in;
    if (!(cin >> L_in >> R_in)) return 0;

    using u64 = unsigned long long;

    u64 U = (u64)R_in * 2ULL;  // 2R
    u64 L2 = (u64)L_in * 2ULL; // 2L

    // Compute bit width: at least 1 bit even for zero
    u64 M = max(U, L2);
    int baseBits = 0;
    if (M == 0) baseBits = 1;
    else {
        while (M) { baseBits++; M >>= 1; }
    }
    // Extra bits to safely drain carries/borrows
    int W = baseBits + 2;

    // DP over states (cS, bu, cDL, bs) each in {0,1} -> 16 states
    // We'll use 1D arrays indexed by: id = cS | (bu<<1) | (cDL<<2) | (bs<<3)
    array<u64, 16> dp{}, nxt{};
    dp[0] = 1; // cS=0, bu=0, cDL=0, bs=0

    for (int i = 0; i < W; ++i) {
        fill(nxt.begin(), nxt.end(), 0);
        int u = (U >> i) & 1ULL;
        int l = (L2 >> i) & 1ULL;

        for (int id = 0; id < 16; ++id) {
            u64 cur = dp[id];
            if (!cur) continue;
            int cS = (id >> 0) & 1;
            int bu = (id >> 1) & 1;
            int cDL = (id >> 2) & 1;
            int bs = (id >> 3) & 1;

            if (i == 0) {
                // Parity constraint: s0 = d0 -> allowed (0,0) and (1,1)
                for (int t = 0; t <= 1; ++t) {
                    int si = t, di = t;

                    int sum1 = si + di + cS;
                    int Sbit = sum1 & 1;
                    int cS2 = sum1 >> 1;
                    int bu2 = ((bu + Sbit) > u) ? 1 : 0;

                    int sum2 = di + l + cDL;
                    int Xbit = sum2 & 1;
                    int cDL2 = sum2 >> 1;
                    int bs2 = ((bs + Xbit) > si) ? 1 : 0;

                    int id2 = (cS2) | (bu2 << 1) | (cDL2 << 2) | (bs2 << 3);
                    nxt[id2] += cur;
                }
            } else {
                // di <= si -> allowed (0,0), (1,0), (1,1)
                // (0,1) is not allowed
                const int pairs[3][2] = {{0,0},{1,0},{1,1}};
                for (int k = 0; k < 3; ++k) {
                    int si = pairs[k][0], di = pairs[k][1];

                    int sum1 = si + di + cS;
                    int Sbit = sum1 & 1;
                    int cS2 = sum1 >> 1;
                    int bu2 = ((bu + Sbit) > u) ? 1 : 0;

                    int sum2 = di + l + cDL;
                    int Xbit = sum2 & 1;
                    int cDL2 = sum2 >> 1;
                    int bs2 = ((bs + Xbit) > si) ? 1 : 0;

                    int id2 = (cS2) | (bu2 << 1) | (cDL2 << 2) | (bs2 << 3);
                    nxt[id2] += cur;
                }
            }
        }
        dp.swap(nxt);
    }

    // To be safe and exact, require final borrows to be zero AND no leftover carries.
    // This ensures the implicit extra top bit (beyond W-1) is zero for the sums we compared.
    u64 ans = dp[0 /*cS=0*/ | (0<<1) /*bu=0*/ | (0<<2) /*cDL=0*/ | (0<<3) /*bs=0*/];

    cout << ans << '\n';
    return 0;
}