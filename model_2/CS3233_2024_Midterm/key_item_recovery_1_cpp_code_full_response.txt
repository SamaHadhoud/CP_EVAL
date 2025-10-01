```cpp
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    int n;
    vector<int> p, sz;
    DSU(int n = 0) : n(n), p(n + 1), sz(n + 1, 1) {
        for (int i = 1; i <= n; ++i) p[i] = i;
    }
    int find(int x) {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> A(N + 1, vector<int>(N + 1));
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) cin >> A[i][j];
    }

    DSU dsu(N);
    vector<pair<int,int>> edges;
    edges.reserve(N - 1);

    vector<int> rep(N + 1, -1);
    vector<int> mark(N + 1, 0);
    int curMark = 1;

    for (int t = N; t >= 1; --t) {
        ++curMark;
        vector<int> roots;
        roots.reserve(N - t);
        for (int j = t + 1; j <= N; ++j) {
            if (A[t][j] == t) {
                int r = dsu.find(j);
                if (mark[r] != curMark) {
                    mark[r] = curMark;
                    rep[r] = j;
                    roots.push_back(r);
                }
            }
        }
        for (int r : roots) {
            int j = rep[r];
            edges.emplace_back(t, j);
            dsu.unite(t, j);
        }
    }

    for (auto &e : edges) {
        cout << e.first << ' ' << e.second << '\n';
    }
    return 0;
}
```