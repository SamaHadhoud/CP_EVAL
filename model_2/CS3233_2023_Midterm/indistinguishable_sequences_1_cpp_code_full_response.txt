```cpp
#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

const int64 MOD = 1000003233LL;

struct FenwickMax {
    int n;
    vector<int> bit;
    FenwickMax(int n=0): n(n), bit(n+1, 0) {}
    void reset(int n_) { n = n_; bit.assign(n+1, 0); }
    void update(int idx, int val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] = max(bit[idx], val);
    }
    int query(int idx) const {
        int res = 0;
        for (; idx > 0; idx -= idx & -idx) res = max(res, bit[idx]);
        return res;
    }
};

struct SegTree {
    int n;
    vector<int64> seg, lz;
    SegTree() : n(0) {}
    SegTree(int n_) { init(n_); }
    void init(int n_) {
        n = n_;
        seg.assign(4*n, 0);
        lz.assign(4*n, 1);
    }
    void build(int p, int l, int r, const vector<int64>& a) {
        lz[p] = 1;
        if (l == r) {
            seg[p] = a[l] % MOD;
            return;
        }
        int m = (l + r) >> 1;
        build(p<<1, l, m, a);
        build(p<<1|1, m+1, r, a);
        seg[p] = (seg[p<<1] + seg[p<<1|1]) % MOD;
    }
    void build(const vector<int64>& a) {
        init((int)a.size());
        if (n == 0) return;
        build(1, 0, n-1, a);
    }
    void apply(int p, int64 mult) {
        seg[p] = (seg[p] * mult) % MOD;
        lz[p] = (lz[p] * mult) % MOD;
    }
    void push(int p) {
        if (lz[p] != 1) {
            apply(p<<1, lz[p]);
            apply(p<<1|1, lz[p]);
            lz[p] = 1;
        }
    }
    void update(int p, int l, int r, int ql, int qr, int64 mult) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) { apply(p, mult); return; }
        push(p);
        int m = (l + r) >> 1;
        update(p<<1, l, m, ql, qr, mult);
        update(p<<1|1, m+1, r, ql, qr, mult);
        seg[p] = (seg[p<<1] + seg[p<<1|1]) % MOD;
    }
    void update(int l, int r, int64 mult) {
        if (l > r || n == 0) return;
        update(1, 0, n-1, l, r, mult);
    }
    int64 sumAll() const {
        return (n ? seg[1] : 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n+1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    // Precompute powers of two up to n
    vector<int64> pow2(n+5, 1);
    for (int i = 1; i < (int)pow2.size(); ++i) pow2[i] = (pow2[i-1] << 1) % MOD;

    // Compute f (LIS ending at i) and g (LIS starting at i)
    FenwickMax fw(n), bw(n);
    vector<int> f(n+1, 0), g(n+1, 0);
    for (int i = 1; i <= n; ++i) {
        f[i] = 1 + fw.query(a[i]-1);
        fw.update(a[i], f[i]);
    }
    for (int i = n; i >= 1; --i) {
        int rev = n - a[i] + 1;
        g[i] = 1 + bw.query(rev - 1);
        bw.update(rev, g[i]);
    }

    int L = 0;
    for (int i = 1; i <= n; ++i) L = max(L, f[i]);
    // Build LIS zone layers
    vector<vector<int>> V(L+2);
    int Z = 0;
    for (int i = 1; i <= n; ++i) {
        if (f[i] + g[i] - 1 == L) {
            V[f[i]].push_back(i);
        } else {
            ++Z;
        }
    }

    // Edge case: L == 0 shouldn't happen; but if no LIS zone, answer is 2^n - 1
    if (L == 0) {
        cout << (pow2[n] + MOD - 1) % MOD << '\n';
        return 0;
    }

    // Precompute adjacency intervals between consecutive layers
    // Ls[d][i], Rs[d][i] for i = 1..|V[d]|, values in 1..|V[d+1]|
    vector<vector<int>> Ls(L+2), Rs(L+2);
    for (int d = 1; d < L; ++d) {
        int m = (int)V[d].size();
        int m2 = (int)V[d+1].size();
        Ls[d].assign(m+1, 0);
        Rs[d].assign(m+1, 0);
        if (m == 0 || m2 == 0) continue; // should not happen in LIS zone
        vector<int> idx2 = V[d+1]; // sorted by index already
        vector<int> val2(m2);
        for (int j = 0; j < m2; ++j) val2[j] = a[idx2[j]];
        int pIndex = 0; // number of positions with idx <= current i
        int hi = 0;     // number of positions with val > a[i]
        for (int ii = 1; ii <= m; ++ii) {
            int iidx = V[d][ii-1];
            int aval = a[iidx];
            while (pIndex < m2 && idx2[pIndex] <= iidx) ++pIndex;
            while (hi < m2 && val2[hi] > aval) ++hi;
            int Lb = pIndex + 1; // 1..m2+1
            int Rb = hi;         // 0..m2
            if (Lb > Rb) {
                // This should not happen when restricted to LIS zone; but guard anyway
                // Clamp to an empty/degenerate interval to avoid UB later
                Lb = 1;
                Rb = 0;
            }
            Ls[d][ii] = Lb;
            Rs[d][ii] = Rb;
        }
    }

    // Build block boundaries: always include 1 and L; include every singleton layer inside (excluding 1 and L to avoid duplicates)
    vector<int> B;
    B.push_back(1);
    for (int d = 2; d <= L-1; ++d) if ((int)V[d].size() == 1) B.push_back(d);
    B.push_back(L);

    auto solve_block = [&](int ell, int r) -> int64 {
        int layers_cnt = r - ell + 1;

        // Forward intervals A: [AL[k], AR[k]] in local indices of V[k]
        vector<int> AL(L+2, 0), AR(L+2, 0);
        // Backward intervals B: [BL[k], BR[k]]
        vector<int> BLv(L+2, 0), BRv(L+2, 0);

        bool left_single = ((int)V[ell].size() == 1);
        bool right_single = ((int)V[r].size() == 1);

        // Forward
        AL[ell] = 1;
        AR[ell] = left_single ? 1 : (int)V[ell].size();
        for (int k = ell; k < r; ++k) {
            int lpos = AL[k], rpos = AR[k];
            // From layer k to k+1, adjacency intervals
            int Lnext = Ls[k][lpos];
            int Rnext = Rs[k][rpos];
            AL[k+1] = Lnext;
            AR[k+1] = Rnext;
        }

        // Backward
        BLv[r] = 1;
        BRv[r] = right_single ? 1 : (int)V[r].size();
        for (int k = r-1; k >= ell; --k) {
            int m = (int)V[k].size();
            // lower = first i with R[k][i] >= BLv[k+1]
            auto bR = Rs[k].begin() + 1;
            auto eR = Rs[k].begin() + m + 1;
            int lower_i = (int)(lower_bound(bR, eR, BLv[k+1]) - bR) + 1; // in [1..m+1]
            // upper = last i with L[k][i] <= BRv[k+1]
            auto bL = Ls[k].begin() + 1;
            auto eL = Ls[k].begin() + m + 1;
            int upper_i = (int)(upper_bound(bL, eL, BRv[k+1]) - bL); // in [0..m]
            if (lower_i > m) lower_i = m+1; // empty possible
            if (upper_i < 1) upper_i = 0;
            BLv[k] = lower_i;
            BRv[k] = upper_i;
        }

        // S(k) = intersection of forward/backward intervals
        vector<int> SL(L+2, 0), SR(L+2, -1);
        for (int k = ell; k <= r; ++k) {
            int l = max(AL[k], BLv[k]);
            int rr = min(AR[k], BRv[k]);
            SL[k] = l;
            SR[k] = rr;
            // In a valid LIS block, l <= rr must hold
            if (l > rr) return 0; // no s->t path (shouldn't happen)
        }

        // DP over layers
        // Initial dp at layer ell (after deciding on layer ell)
        int m0 = SR[ell] - SL[ell] + 1;
        if (m0 <= 0) return 0;

        vector<int64> dp(m0 + 1, 0); // t=0..m0
        if (left_single) {
            dp[0] = 1;
            dp[1] = 1;
        } else {
            dp[0] = 1;
            for (int j = 1; j <= m0; ++j) dp[j] = pow2[j-1];
        }

        for (int k = ell; k < r; ++k) {
            int mcur = SR[k] - SL[k] + 1;
            int mnext = SR[k+1] - SL[k+1] + 1;

            // Build R array restricted to S(k+1), reindexed
            vector<int> Rarr(mcur + 1, 0); // 1..mcur
            for (int i = 1; i <= mcur; ++i) {
                int iorig = SL[k] + i - 1;
                int lraw = Ls[k][iorig];
                int rraw = Rs[k][iorig];
                int lint = max(lraw, SL[k+1]);
                int rint = min(rraw, SR[k+1]);
                // In S restriction, must be non-empty
                int rloc = rint - SL[k+1] + 1; // in [1..mnext]
                Rarr[i] = rloc;
            }

            // Prepare posList by R value
            vector<vector<int>> posList(mnext + 1);
            for (int i = 1; i <= mcur; ++i) posList[Rarr[i]].push_back(i);

            // Precompute total = sum_t dp[t] * 2^t
            int64 total = 0;
            for (int t = 0; t <= mcur; ++t) {
                total += (dp[t] * pow2[t]) % MOD;
                if (total >= MOD) total -= MOD;
            }

            // Build segtree with base = dp[t] (t=0..mcur)
            vector<int64> base(mcur + 1, 0);
            for (int t = 0; t <= mcur; ++t) base[t] = dp[t];
            SegTree st(mcur + 1);
            st.build(base);

            // Compute SC[s] = sum_t dp[t] * 2^{C_t(s)} for s = 0..mnext-1
            vector<int64> SC(mnext, 0);
            if (mnext >= 1) {
                SC[0] = st.sumAll();
                for (int s = 1; s <= mnext - 1; ++s) {
                    for (int p : posList[s]) {
                        // Multiply suffix [p..mcur] by 2
                        st.update(p, mcur, 2);
                    }
                    SC[s] = st.sumAll();
                }
            }

            // Transition
            vector<int64> dpNext(mnext + 1, 0);
            dpNext[0] = total;
            for (int j = 1; j <= mnext; ++j) {
                int s = j - 1;
                int64 sub = (s >= 0 ? SC[s] : 0);
                int64 diff = total - sub;
                if (diff < 0) diff += MOD;
                dpNext[j] = (pow2[j-1] * (diff % MOD)) % MOD;
            }
            dp.swap(dpNext);
        }

        // Finalization at layer r
        int mlast = SR[r] - SL[r] + 1;
        int64 ansBlock = 0;
        if (right_single) {
            // Sink is unique; S(r) should be size 1 after restriction
            // In reindexed coordinates, its position is 1
            if ((int)dp.size() > 1) ansBlock = dp[1];
            else ansBlock = 0;
        } else {
            for (int j = 1; j <= mlast; ++j) {
                ansBlock += dp[j];
                if (ansBlock >= MOD) ansBlock -= MOD;
            }
        }
        return ansBlock % MOD;
    };

    // Multiply answers over all blocks and factor 2^Z for non-LIS-zone indices
    int64 ans = pow2[Z];
    for (int i = 0; i + 1 < (int)B.size(); ++i) {
        int ell = B[i];
        int r = B[i+1];
        int64 blockAns = solve_block(ell, r);
        ans = (ans * blockAns) % MOD;
    }

    cout << ans % MOD << '\n';
    return 0;
}
```