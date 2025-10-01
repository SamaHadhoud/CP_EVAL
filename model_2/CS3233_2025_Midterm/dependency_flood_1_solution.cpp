#include <bits/stdc++.h>
using namespace std;

static inline uint64_t make_key(int u, int v) {
    return (static_cast<uint64_t>(u) << 32) | static_cast<uint32_t>(v);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    if (!(cin >> N >> M >> K)) return 0;

    vector<vector<int>> out(N + 1);
    vector<int> dist(N + 1, 1);
    unordered_set<uint64_t> edges;
    edges.reserve((size_t)M * 2 + 16);

    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        uint64_t key = make_key(u, v);
        if (edges.insert(key).second) {
            out[u].push_back(v);
        }
    }

    // Initial DP for longest path lengths (in vertices)
    for (int u = 1; u <= N; ++u) {
        int du = dist[u];
        for (int v : out[u]) {
            if (dist[v] < du + 1) dist[v] = du + 1;
        }
    }

    int Q;
    cin >> Q;
    edges.reserve(edges.size() + (size_t)Q + 16);

    vector<int> tempVal(N + 1, 0);
    vector<int> lastSeen(N + 1, 0);
    int timer = 0;
    vector<int> q;
    vector<int> touched;
    q.reserve(1024);
    touched.reserve(1024);

    string acc = "accept\n";
    string rej = "reject\n";

    for (int qi = 0; qi < Q; ++qi) {
        int u, v;
        cin >> u >> v;
        uint64_t key = make_key(u, v);

        // 1) Duplicate edge: accept, no change.
        if (edges.find(key) != edges.end()) {
            cout << acc;
            continue;
        }

        // 2) Edge doesn't improve v: accept and add the edge.
        if (dist[u] + 1 <= dist[v]) {
            edges.insert(key);
            out[u].push_back(v);
            cout << acc;
            continue;
        }

        // 3) If dist[u] == K, then v must be at least K+1 -> reject.
        if (dist[u] == K) {
            cout << rej;
            continue;
        }

        // 4) Propagate forced increases starting from v.
        ++timer;
        q.clear();
        touched.clear();

        int startVal = dist[u] + 1; // <= K here
        tempVal[v] = startVal;
        lastSeen[v] = timer;
        q.push_back(v);
        touched.push_back(v);

        bool rejected = false;
        size_t head = 0;
        while (head < q.size() && !rejected) {
            int x = q[head++];
            int valX = (lastSeen[x] == timer) ? tempVal[x] : dist[x];
            if (valX > K) { rejected = true; break; }

            int candNext = valX + 1;
            if (candNext > K) { rejected = true; break; }

            for (int y : out[x]) {
                int curY = (lastSeen[y] == timer) ? tempVal[y] : dist[y];
                if (candNext > curY) {
                    tempVal[y] = candNext;
                    if (lastSeen[y] != timer) {
                        lastSeen[y] = timer;
                        touched.push_back(y);
                    }
                    q.push_back(y);
                }
            }
        }

        if (rejected) {
            cout << rej;
            continue;
        }

        // Commit changes
        for (int x : touched) dist[x] = tempVal[x];

        // Add the new edge
        edges.insert(key);
        out[u].push_back(v);
        cout << acc;
    }

    return 0;
}