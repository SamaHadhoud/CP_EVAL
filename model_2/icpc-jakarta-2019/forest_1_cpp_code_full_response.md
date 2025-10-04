```cpp
#include <bits/stdc++.h>
using namespace std;

struct H {
    uint64_t a, b;
    bool operator==(const H& o) const { return a == o.a && b == o.b; }
    bool operator<(const H& o) const { return a < o.a || (a == o.a && b < o.b); }
};

static inline uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

struct Canonicalizer {
    const vector<vector<int>>& g;
    vector<char>* inComp; // pointer to mask of current component

    Canonicalizer(const vector<vector<int>>& graph) : g(graph), inComp(nullptr) {}

    void setMask(vector<char>* mask) { inComp = mask; }

    H combineList(const vector<H>& ch) const {
        uint64_t A = 0x6a09e667f3bcc909ULL; // sqrt(2)
        uint64_t B = 0xbb67ae8584caa73bULL; // sqrt(3)
        A ^= splitmix64((uint64_t)ch.size() + 0x9e3779b97f4a7c15ULL);
        B ^= splitmix64((uint64_t)ch.size() * 0x100000001b3ULL + 0x243f6a8885a308d3ULL);
        for (const auto& h : ch) {
            uint64_t v1 = h.a + 0x9e3779b97f4a7c15ULL;
            uint64_t v2 = h.b + 0x243f6a8885a308d3ULL;
            A = splitmix64(A ^ v1);
            A = splitmix64(A ^ v2);
            B = splitmix64(B ^ v2);
            B = splitmix64(B ^ v1);
        }
        A = splitmix64(A ^ 0xA5A5A5A5A5A5A5A5ULL);
        B = splitmix64(B ^ 0x5A5A5A5A5A5A5A5AULL);
        return {A, B};
    }

    H encodeRooted(int x, int p) const {
        vector<H> child;
        child.reserve(g[x].size());
        for (int y : g[x]) {
            if (y == p) continue;
            if ((*inComp)[y]) {
                child.push_back(encodeRooted(y, x));
            }
        }
        sort(child.begin(), child.end());
        return combineList(child);
    }
};

H canonical_component(const vector<int>& comp,
                      vector<char>& in_comp,
                      vector<int>& deg,
                      vector<char>& removed,
                      Canonicalizer& iso) {
    // mark component nodes
    for (int v : comp) {
        in_comp[v] = 1;
        removed[v] = 0;
        deg[v] = 0;
    }
    // compute degrees within component
    for (int v : comp) {
        for (int w : iso.g[v]) if (in_comp[w]) deg[v]++;
    }
    // find centers by leaf trimming
    queue<int> q;
    for (int v : comp) if (deg[v] <= 1) q.push(v);
    int rem = (int)comp.size();
    while (rem > 2 && !q.empty()) {
        int v = q.front(); q.pop();
        if (removed[v]) continue;
        removed[v] = 1;
        rem--;
        for (int w : iso.g[v]) {
            if (in_comp[w] && !removed[w]) {
                deg[w]--;
                if (deg[w] == 1) q.push(w);
            }
        }
    }
    vector<int> centers;
    centers.reserve(2);
    for (int v : comp) if (!removed[v]) centers.push_back(v);

    iso.setMask(&in_comp);
    H res;
    if (centers.size() == 1) {
        res = iso.encodeRooted(centers[0], 0);
    } else {
        H h1 = iso.encodeRooted(centers[0], centers[1]);
        H h2 = iso.encodeRooted(centers[1], centers[0]);
        if (h2 < h1) swap(h1, h2);
        vector<H> pairv = {h1, h2};
        res = iso.combineList(pairv);
    }

    // unmark component nodes
    for (int v : comp) in_comp[v] = 0;
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> g(N + 1);
    for (int i = 0; i < N - 1; ++i) {
        int a, b; cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    for (int i = 1; i <= N; ++i) sort(g[i].begin(), g[i].end());

    Canonicalizer iso(g);
    vector<char> visited(N + 1, 0), in_comp(N + 1, 0), removed(N + 1, 0);
    vector<int> deg(N + 1, 0);

    int ans = -1;
    for (int u = 1; u <= N; ++u) {
        if ((int)g[u].size() < 2) continue;
        fill(visited.begin(), visited.end(), 0);

        vector<H> encs;
        encs.reserve(g[u].size());
        for (int v0 : g[u]) {
            if (visited[v0]) continue;
            // BFS to collect component ignoring u
            vector<int> comp;
            comp.reserve(N);
            queue<int> q;
            visited[v0] = 1;
            q.push(v0);
            comp.push_back(v0);
            while (!q.empty()) {
                int x = q.front(); q.pop();
                for (int y : g[x]) {
                    if (y == u) continue;
                    if (!visited[y]) {
                        visited[y] = 1;
                        q.push(y);
                        comp.push_back(y);
                    }
                }
            }
            // Compute unrooted canonical encoding of this component
            H h = canonical_component(comp, in_comp, deg, removed, iso);
            encs.push_back(h);
        }
        bool ok = true;
        for (size_t i = 1; i < encs.size(); ++i) {
            if (!(encs[i] == encs[0])) { ok = false; break; }
        }
        if (ok) ans = max<int>(ans, (int)encs.size());
    }

    cout << ans << '\n';
    return 0;
}
```