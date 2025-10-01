#include <bits/stdc++.h>
using namespace std;

// Iterative DSU for "next free" positions on 1..N+1
struct NextFree {
    int n;
    vector<int> p;
    NextFree(int n=0): n(n), p(n+2) {
        for (int i = 1; i <= n+1; ++i) p[i] = i;
    }
    inline int find(int x) {
        int r = x;
        while (p[r] != r) r = p[r];
        while (p[x] != x) { int nx = p[x]; p[x] = r; x = nx; }
        return r;
    }
    inline void remove_pos(int pos) { // mark pos as used
        int r = find(pos);
        p[r] = find(r + 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<int> A(N + 1);
    for (int i = 1; i <= N; ++i) cin >> A[i];

    vector<vector<int>> ch(N + 1);
    for (int i = 2; i <= N; ++i) {
        int b; cin >> b;
        ch[b].push_back(i);
    }

    // Euler tour (preorder) + postorder without recursion
    vector<int> tin(N + 1), tout(N + 1), ord(N + 1), post;
    post.reserve(N);
    int timer = 0;
    vector<pair<int,int>> st;
    st.reserve(N);
    st.emplace_back(1, 0);
    while (!st.empty()) {
        auto &top = st.back();
        int v = top.first;
        int &i = top.second;
        if (i == 0) {
            tin[v] = ++timer;
            ord[timer] = v;
        }
        if (i < (int)ch[v].size()) {
            int u = ch[v][i++];
            st.emplace_back(u, 0);
        } else {
            tout[v] = timer;
            post.push_back(v);
            st.pop_back();
        }
    }

    // DP bottom-up
    vector<int> dp(N + 1), sz(N + 1), needExtra(N + 1, 0);
    vector<char> choose(N + 1, 0);

    for (int idx = 0; idx < (int)post.size(); ++idx) {
        int v = post[idx];
        long long sum = 0;
        int szv = 1;
        for (int u : ch[v]) {
            sum += dp[u];
            szv += sz[u];
        }
        sz[v] = szv;
        int desc = szv - 1;
        int av1 = A[v] + 1;

        if (av1 > desc) {
            choose[v] = 1;
            dp[v] = (int)(1 + sum);
            needExtra[v] = 0;
        } else {
            int cost1 = (int)(1 + sum);
            int cost2 = max((int)sum, av1);
            if (cost1 <= cost2) {
                choose[v] = 1;
                dp[v] = cost1;
                needExtra[v] = 0;
            } else {
                choose[v] = 0;
                dp[v] = cost2;
                needExtra[v] = max(0, av1 - (int)sum);
            }
        }
    }

    int L = dp[1];
    if (K < L) {
        cout << -1 << '\n';
        return 0;
    }

    // Build minimal set S using DSU next-free over Euler positions
    NextFree dsu(N);
    vector<char> selected(N + 1, 0);

    st.clear();
    st.emplace_back(1, 0);
    while (!st.empty()) {
        auto &top = st.back();
        int v = top.first;
        int &i = top.second;

        if (i == 0) {
            // entry event
            if (choose[v]) {
                if (!selected[v]) {
                    selected[v] = 1;
                    int root = dsu.find(tin[v]); // should be tin[v]
                    if (root == tin[v]) dsu.remove_pos(root);
                }
            }
        }
        if (i < (int)ch[v].size()) {
            int u = ch[v][i++];
            st.emplace_back(u, 0);
        } else {
            // exit event
            if (!choose[v]) {
                int need = needExtra[v];
                int cur = dsu.find(tin[v] + 1);
                while (need > 0) {
                    // DP ensures cur <= tout[v]
                    int node = ord[cur];
                    selected[node] = 1;
                    dsu.remove_pos(cur);
                    --need;
                    cur = dsu.find(cur); // next free >= cur
                }
            }
            st.pop_back();
        }
    }

    // Add any K - L additional nodes arbitrarily
    int toAdd = K - L;
    while (toAdd > 0) {
        int pos = dsu.find(1);
        if (pos > N) break; // should not happen if toAdd <= remaining
        int node = ord[pos];
        selected[node] = 1;
        dsu.remove_pos(pos);
        --toAdd;
    }

    // Output: selected first in preorder, then the rest
    bool first = true;
    for (int i = 1; i <= N; ++i) {
        int v = ord[i];
        if (selected[v]) {
            if (!first) cout << ' ';
            first = false;
            cout << v;
        }
    }
    for (int i = 1; i <= N; ++i) {
        int v = ord[i];
        if (!selected[v]) {
            if (!first) cout << ' ';
            first = false;
            cout << v;
        }
    }
    cout << '\n';
    return 0;
}