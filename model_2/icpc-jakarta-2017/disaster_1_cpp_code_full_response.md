#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct DSU {
    vector<int> p, sz;
    DSU(int n = 0) { init(n); }
    void init(int n) {
        p.resize(n);
        sz.assign(n, 1);
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
    }
    bool same(int a, int b) { return find(a) == find(b); }
};

static inline ll clampll(ll v, ll lo, ll hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    ll x1, y1, x2, y2;
    int N;
    if (!(cin >> x1 >> y1 >> x2 >> y2 >> N)) return 0;

    vector<ll> fx(N), fy(N), r(N);
    for (int i = 0; i < N; ++i) {
        cin >> fx[i] >> fy[i] >> r[i];
    }

    // Nodes: 0..N-1 = disks, N=Left, N+1=Right, N+2=Bottom, N+3=Top
    int L = N, R = N + 1, B = N + 2, T = N + 3;
    DSU dsu(N + 4);

    // Disk-disk overlaps (strictly overlapping interiors)
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            ll dx = fx[i] - fx[j];
            ll dy = fy[i] - fy[j];
            ll s = r[i] + r[j];
            if (dx * dx + dy * dy < s * s) {
                dsu.unite(i, j);
            }
        }
    }

    // Disk-side intersections (distance to segment strictly less than r)
    for (int i = 0; i < N; ++i) {
        ll ri2 = r[i] * r[i];

        // Left side x = x1, y in [y1, y2]
        {
            ll y0 = clampll(fy[i], y1, y2);
            ll dx = x1 - fx[i];
            ll dy = y0 - fy[i];
            if (dx * dx + dy * dy < ri2) dsu.unite(i, L);
        }
        // Right side x = x2
        {
            ll y0 = clampll(fy[i], y1, y2);
            ll dx = x2 - fx[i];
            ll dy = y0 - fy[i];
            if (dx * dx + dy * dy < ri2) dsu.unite(i, R);
        }
        // Bottom side y = y1, x in [x1, x2]
        {
            ll x0 = clampll(fx[i], x1, x2);
            ll dx = x0 - fx[i];
            ll dy = y1 - fy[i];
            if (dx * dx + dy * dy < ri2) dsu.unite(i, B);
        }
        // Top side y = y2
        {
            ll x0 = clampll(fx[i], x1, x2);
            ll dx = x0 - fx[i];
            ll dy = y2 - fy[i];
            if (dx * dx + dy * dy < ri2) dsu.unite(i, T);
        }
    }

    bool blocked =
        dsu.same(L, R) ||  // left-right wall
        dsu.same(B, T) ||  // bottom-top wall
        dsu.same(L, B) ||  // bottom-left corner trapped
        dsu.same(R, T);    // top-right corner trapped

    cout << (blocked ? "NO" : "YES") << '\n';
    return 0;
}