#include <bits/stdc++.h>
using namespace std;

struct Frame {
    int u, parent, peid, it;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C;
    if (!(cin >> C)) return 0;
    while (C--) {
        int n, m;
        cin >> n >> m;

        vector<int> a(m + 1);
        long long sumA = 0;
        for (int i = 1; i <= m; ++i) {
            cin >> a[i];
            sumA += a[i];
        }

        vector<vector<pair<int,int>>> adj(n + 1);
        vector<int> U(m + 1), V(m + 1);
        for (int i = 1; i <= m; ++i) {
            int u, v;
            cin >> u >> v;
            U[i] = u; V[i] = v;
            adj[u].push_back({v, i});
            adj[v].push_back({u, i});
        }

        // Build a spanning tree and get postorder list of its edges (L).
        vector<char> vis(n + 1, 0);
        vector<int> L;
        L.reserve(n - 1);
        vector<Frame> st;
        st.reserve(n);

        vis[1] = 1;
        st.push_back({1, 0, 0, 0});
        while (!st.empty()) {
            Frame &f = st.back();
            if (f.it < (int)adj[f.u].size()) {
                auto [v, eid] = adj[f.u][f.it++];
                if (v == f.parent) continue;
                if (!vis[v]) {
                    vis[v] = 1;
                    st.push_back({v, f.u, eid, 0});
                }
            } else {
                if (f.peid != 0) L.push_back(f.peid);
                st.pop_back();
            }
        }

        long long need = (n + 1LL) / 2LL;
        long long S = min(sumA, (long long)(n - 1));
        if (S < need) {
            cout << "No\n";
            continue;
        }

        cout << "Yes\n";
        vector<int> ans(m + 1, 0);
        int p = (int)L.size() - 1;
        for (int c = 1; c <= m && S > 0; ++c) {
            int t = (int)min<long long>(a[c], S);
            for (int i = 0; i < t; ++i) {
                ans[L[p]] = c;
                --p;
            }
            S -= t;
        }

        for (int i = 1; i <= m; ++i) {
            if (i > 1) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }

    return 0;
}