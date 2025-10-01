```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<ll> S(N + 1);
    for (int i = 1; i <= N; ++i) cin >> S[i];
    vector<vector<int>> g(N + 1);
    for (int i = 0; i < M; ++i) {
        int u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    // Color the graph and detect bipartiteness
    vector<int> col(N + 1, -1);
    bool nonBip = false;
    queue<int> q;
    col[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (col[v] == -1) {
                col[v] = col[u] ^ 1;
                q.push(v);
            } else if (col[v] == col[u]) {
                nonBip = true;
            }
        }
    }
    
    int Q; cin >> Q;

    // If non-bipartite, every Type-1 query answers 0; updates don't affect answers.
    if (nonBip) {
        for (int qi = 0; qi < Q; ++qi) {
            int A, B; ll C;
            cin >> A >> B >> C;
            if (A == 0) {
                S[B] = C; // keep values consistent though it doesn't matter for answers
            } else {
                cout << 0 << '\n';
            }
        }
        return 0;
    }

    // Bipartite case: maintain global ordered set and multiset of cross-color adjacent gaps.
    set<pair<ll,int>> U;          // (value, id)
    multiset<ll> gaps;            // gaps across adjacent pairs with different colors

    auto add_boundary = [&](const pair<ll,int>& a, const pair<ll,int>& b) {
        if (col[a.second] != col[b.second]) {
            ll d = llabs(b.first - a.first);
            gaps.insert(d);
        }
    };
    auto remove_boundary = [&](const pair<ll,int>& a, const pair<ll,int>& b) {
        if (col[a.second] != col[b.second]) {
            ll d = llabs(b.first - a.first);
            auto it = gaps.find(d);
            if (it != gaps.end()) gaps.erase(it);
            // else: should not happen if invariants are maintained
        }
    };

    for (int i = 1; i <= N; ++i) U.insert({S[i], i});
    if (U.size() >= 2) {
        auto it = U.begin();
        auto prv = it++;
        for (; it != U.end(); ++it, ++prv) {
            add_boundary(*prv, *it);
        }
    }

    auto update_value = [&](int idx, ll newS) {
        // Remove old entry
        auto it = U.find({S[idx], idx});
        if (it == U.end()) return; // safety
        // Remove boundaries touching it
        if (it != U.begin()) {
            auto pit = it; --pit;
            remove_boundary(*pit, *it);
        }
        {
            auto nit = it; ++nit;
            if (nit != U.end()) {
                remove_boundary(*it, *nit);
            }
        }
        // After removal, link predecessor and successor
        pair<ll,int> leftNeighbor, rightNeighbor;
        bool hasLeft = false, hasRight = false;
        if (it != U.begin()) {
            auto pit = it; --pit;
            leftNeighbor = *pit; hasLeft = true;
        }
        {
            auto nit = it; ++nit;
            if (nit != U.end()) {
                rightNeighbor = *nit; hasRight = true;
            }
        }
        U.erase(it);
        if (hasLeft && hasRight) add_boundary(leftNeighbor, rightNeighbor);

        // Prepare insertion: find neighbors at insertion position and remove their boundary
        auto pos = U.lower_bound({newS, idx});
        pair<ll,int> left2, right2;
        bool hasLeft2 = false, hasRight2 = false;
        if (pos != U.begin()) {
            auto pit = pos; --pit;
            left2 = *pit; hasLeft2 = true;
        }
        if (pos != U.end()) {
            right2 = *pos; hasRight2 = true;
        }
        if (hasLeft2 && hasRight2) remove_boundary(left2, right2);

        // Insert new entry and add new boundaries
        U.insert({newS, idx});
        auto itNew = U.find({newS, idx});
        if (itNew != U.begin()) {
            auto pit = itNew; --pit;
            add_boundary(*pit, *itNew);
        }
        {
            auto nit = itNew; ++nit;
            if (nit != U.end()) {
                add_boundary(*itNew, *nit);
            }
        }
        S[idx] = newS;
    };

    for (int qi = 0; qi < Q; ++qi) {
        int A, B; ll C;
        cin >> A >> B >> C;
        if (A == 0) {
            update_value(B, C);
        } else {
            // A == 1: query starting cities B and C
            if (col[B] == col[(int)C]) {
                cout << 0 << '\n';
            } else {
                // Different parts: answer is min gap across adjacent cross-color pairs
                if (gaps.empty()) cout << 0 << '\n'; // safety
                else cout << *gaps.begin() << '\n';
            }
        }
    }

    return 0;
}
```