```cpp
#include <bits/stdc++.h>
using namespace std;

// We compute D2(H) via smallest-last edge peeling on the complement graph H.
// Answer k = 2*(N-1) - D2(H).

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    
    vector<vector<char>> adj(N, vector<char>(N, 0));
    vector<int> degG(N, 0);
    for (int i = 0; i < M; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        adj[a][b] = adj[b][a] = 1;
        degG[a]++; degG[b]++;
    }
    
    // Build complement graph H edges (missing edges of G)
    vector<pair<int,int>> edges; edges.reserve(N*(N-1)/2 - M);
    vector<vector<int>> inc(N);
    for (int i = 0; i < N; ++i) {
        inc[i].reserve(N);
    }
    for (int i = 0; i < N; ++i) {
        for (int j = i+1; j < N; ++j) {
            if (!adj[i][j]) {
                int id = (int)edges.size();
                edges.emplace_back(i, j);
                inc[i].push_back(id);
                inc[j].push_back(id);
            }
        }
    }
    
    int Ebar = (int)edges.size();
    // Problem guarantees the graph is not complete: Ebar > 0.
    if (Ebar == 0) {
        // Should not happen per constraints; fallback to maximum possible k.
        cout << max(0, 2*(N-1) - 0) << '\n';
        return 0;
    }
    
    vector<int> degH(N);
    for (int v = 0; v < N; ++v) degH[v] = (N - 1) - degG[v];
    
    // Buckets for s(e) = degH(u) + degH(v), range [0..2N-2]
    int maxS = 2*(N - 1);
    int B = maxS + 1;
    
    vector<int> s(Ebar, 0);
    vector<int> head(B, -1), nxt(Ebar, -1), prv(Ebar, -1);
    vector<char> present(Ebar, 1);
    
    auto addToBucket = [&](int id, int &minS) {
        int b = s[id];
        int h = head[b];
        prv[id] = -1;
        nxt[id] = h;
        if (h != -1) prv[h] = id;
        head[b] = id;
        if (b < minS) minS = b;
    };
    auto removeFromBucket = [&](int id) {
        int b = s[id];
        int p = prv[id];
        int n = nxt[id];
        if (p == -1) head[b] = n;
        else nxt[p] = n;
        if (n != -1) prv[n] = p;
        prv[id] = nxt[id] = -1;
    };
    
    int minS = maxS;
    for (int id = 0; id < Ebar; ++id) {
        int u = edges[id].first, v = edges[id].second;
        s[id] = degH[u] + degH[v];
        addToBucket(id, minS);
    }
    
    int answerT = 0;
    int removed = 0;
    
    while (removed < Ebar) {
        while (minS <= maxS && head[minS] == -1) ++minS;
        int id = head[minS];
        // Remove this edge (current minimum s)
        answerT = max(answerT, s[id]);
        present[id] = 0;
        removeFromBucket(id);
        ++removed;
        
        int u = edges[id].first, v = edges[id].second;
        // Decrease degrees in H
        degH[u]--; degH[v]--;
        
        // Update s for edges incident to u
        for (int e2 : inc[u]) {
            if (!present[e2]) continue;
            removeFromBucket(e2);
            s[e2] -= 1;
            addToBucket(e2, minS);
        }
        // Update s for edges incident to v
        for (int e2 : inc[v]) {
            if (!present[e2]) continue;
            removeFromBucket(e2);
            s[e2] -= 1;
            addToBucket(e2, minS);
        }
    }
    
    int k = 2*(N - 1) - answerT;
    cout << k << '\n';
    return 0;
}
```