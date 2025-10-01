```cpp
#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

// Extended GCD for modular inverse
static inline int64 modinv(int64 a, int64 mod) {
    if (mod == 1) return 0;
    int64 b = mod, u = 1, v = 0;
    while (b) {
        int64 t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    // now a = gcd(original a, mod) = 1
    u %= mod;
    if (u < 0) u += mod;
    return u;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    int Q;
    if (!(cin >> N >> Q)) return 0;

    vector<int> P(N + 1);
    for (int i = 1; i <= N; ++i) cin >> P[i];

    // Cycle decomposition
    vector<int> vis(N + 1, 0);
    vector<vector<int>> cycles;
    vector<int> cid(N + 1, -1), posInCycle(N + 1, -1), clen(N + 1, 0);
    for (int i = 1; i <= N; ++i) {
        if (!vis[i]) {
            vector<int> cyc;
            int cur = i;
            while (!vis[cur]) {
                vis[cur] = 1;
                cyc.push_back(cur);
                cur = P[cur];
            }
            int id = (int)cycles.size();
            cycles.push_back(cyc);
            int c = (int)cyc.size();
            for (int p = 0; p < c; ++p) {
                int v = cyc[p];
                cid[v] = id;
                posInCycle[v] = p;
                clen[v] = c;
            }
        }
    }

    // Precompute L_j, d_j, m_j
    vector<int64> L(N + 1, 0), d(N + 1, 0), m(N + 1, 0);
    L[0] = 1;
    for (int j = 1; j <= N; ++j) {
        int c = clen[j];
        int64 g = std::gcd(L[j - 1], (int64)c);
        d[j] = g;
        // L[j] = lcm(L[j-1], c) = L[j-1] / g * c
        L[j] = (L[j - 1] / g) * c;
        m[j] = c / g;
    }
    int64 orderL = L[N]; // this is M - 1

    // B_j = L / L_j
    vector<int64> B(N + 1, 0);
    for (int j = 1; j <= N; ++j) {
        B[j] = orderL / L[j];
    }

    // Precompute inverses inv_j of (L_{j-1}/d_j) modulo (c_j/d_j)
    vector<int64> inv(N + 1, 0);
    for (int j = 1; j <= N; ++j) {
        int c = clen[j];
        int64 g = d[j];
        int64 nprime = c / g;
        int64 mprime = L[j - 1] / g;
        if (nprime == 1) inv[j] = 0;
        else inv[j] = modinv(mprime % nprime, nprime);
    }

    // Precompute ord_j: for each j, r0 in [0..d_j-1], store ordering of t in [0..m_j-1]
    // Flattened as ord[j][r0 * m[j] + z] = tindex
    vector<vector<int>> ord(N + 1);
    for (int j = 1; j <= N; ++j) {
        int c = clen[j];
        int g = (int)d[j];
        int mm = (int)m[j];
        ord[j].assign(c, 0); // size = d * m = c
        int cycId = cid[j];
        int p = posInCycle[j];
        const auto &cyc = cycles[cycId];

        for (int r0 = 0; r0 < g; ++r0) {
            vector<pair<int,int>> vec; // (value, t)
            vec.reserve(mm);
            for (int t = 0; t < mm; ++t) {
                int s = r0 + t * g; // residue modulo c
                int val = cyc[(p + s) % c]; // value (P^s)_j
                vec.emplace_back(val, t);
            }
            sort(vec.begin(), vec.end());
            for (int z = 0; z < mm; ++z) {
                ord[j][r0 * mm + z] = vec[z].second;
            }
        }
    }

    // Answer queries
    // For each K: compute z_j via mixed-radix with weights B[j], then reconstruct r using CRT
    while (Q--) {
        int64 K;
        cin >> K;
        int64 rem = K - 1;

        int64 r = 0; // residue modulo L_{j-1}, starts at 0 (P^0)
        for (int j = 1; j <= N; ++j) {
            int64 z = B[j] ? (rem / B[j]) : 0; // z in [0..m[j]-1]
            if (B[j]) rem %= B[j];

            int64 g = d[j];
            int c = clen[j];
            int mm = (int)m[j];
            int64 r0_small = (g ? (r % g) : 0);
            int tindex = ord[j][(int)(r0_small * mm + z)];
            int64 s = r0_small + g * tindex; // residue modulo c

            int64 mprev = L[j - 1];
            int64 Lj = L[j];

            int64 nprime = c / g;
            if (nprime != 1) {
                int64 diff = s - r;           // divisible by g
                int64 diff_q = diff / g;      // may be negative
                int64 t = ( (__int128)((diff_q % nprime + nprime) % nprime) * inv[j]) % nprime;
                r = r + mprev * t;
                if (r >= Lj) r %= Lj;
            } else {
                // unique merge
                if (r >= Lj) r %= Lj;
            }
        }

        // Map 0 -> L (i.e., P^0 == P^L), otherwise r itself
        if (r == 0) cout << orderL << '\n';
        else cout << r << '\n';
    }

    return 0;
}
```