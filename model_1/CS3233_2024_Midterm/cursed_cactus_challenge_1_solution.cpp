#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int N = 1e5 + 5;
using ll = long long;

struct State {
    int c;
    int s;
    int ss;
    State(int c = 0, int s = 0, int ss = 0) : c(c), s(s), ss(ss) {}
};

State operator+(const State& a, const State& b) {
    return State((a.c + b.c) % MOD, (a.s + b.s) % MOD, (a.ss + b.ss) % MOD);
}

State operator*(const State& a, const State& b) {
    int c = (ll)a.c * b.c % MOD;
    int s = ((ll)a.s * b.c % MOD + (ll)a.c * b.s % MOD) % MOD;
    int ss = ((ll)a.ss * b.c % MOD + (ll)a.c * b.ss % MOD + 2LL * a.s % MOD * b.s % MOD) % MOD;
    return State(c, s, ss);
}

int n, m;
int a[N];
vector<int> g[N];
int parent[N], depth[N];
State dp[N][2];
bool visited[N];
set<pair<int, int>> cycle_edges;
vector<vector<int>> cycles_for_node[N];

State linear_chain(const vector<int>& chain, bool force_first, bool force_last) {
    int n = chain.size();
    if (n == 0) {
        return State(1, 0, 0);
    }
    State f0, f1;
    if (force_first) {
        f0 = dp[chain[0]][0];
        f1 = State(0, 0, 0);
    } else {
        f0 = dp[chain[0]][0];
        f1 = dp[chain[0]][1];
    }

    for (int i = 1; i < n; i++) {
        State next_f0 = (f0 + f1) * dp[chain[i]][0];
        State next_f1 = f0 * dp[chain[i]][1];
        f0 = next_f0;
        f1 = next_f1;
    }

    if (force_last) {
        return f0;
    } else {
        return f0 + f1;
    }
}

void dfs(int u, int p) {
    visited[u] = true;
    depth[u] = (p == -1 ? 0 : depth[p] + 1);
    parent[u] = p;
    dp[u][0] = State(1, 0, 0);
    dp[u][1] = State(1, a[u], (ll)a[u] * a[u] % MOD);

    for (int v : g[u]) {
        if (v == p) continue;
        if (!visited[v]) {
            dfs(v, u);
        } else if (depth[v] < depth[u]) {
            vector<int> cycle;
            int cur = u;
            while (cur != v) {
                cycle.push_back(cur);
                cur = parent[cur];
            }
            cycle.push_back(v);
            cycles_for_node[v].push_back(cycle);
            for (int i = 0; i < (int)cycle.size(); i++) {
                int uu = cycle[i];
                int vv = (i + 1 < cycle.size()) ? cycle[i+1] : cycle[0];
                if (uu > vv) swap(uu, vv);
                cycle_edges.insert({uu, vv});
            }
        }
    }

    for (int v : g[u]) {
        if (v == p) continue;
        if (depth[v] == depth[u] + 1) {
            pair<int, int> edge = {min(u, v), max(u, v)};
            if (cycle_edges.find(edge) != cycle_edges.end()) {
                continue;
            }
            dp[u][0] = dp[u][0] * (dp[v][0] + dp[v][1]);
            dp[u][1] = dp[u][1] * dp[v][0];
        }
    }

    for (auto& cycle : cycles_for_node[u]) {
        vector<int> chain = cycle;
        chain.pop_back();
        reverse(chain.begin(), chain.end());
        State total_chain = linear_chain(chain, false, false);
        State safe_chain = linear_chain(chain, true, true);
        dp[u][0] = dp[u][0] * total_chain;
        dp[u][1] = dp[u][1] * safe_chain;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    dfs(1, -1);

    State total = dp[1][0] + dp[1][1];
    cout << total.ss << endl;

    return 0;
}