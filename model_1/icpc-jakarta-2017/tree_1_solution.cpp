#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

const int MAXN = 100000;
const int MAX_LOG = 17;

vector<int> graph[MAXN+5];
int depth[MAXN+5];
int parent0[MAXN+5];
int parent_table[MAX_LOG+1][MAXN+5];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    for (int i=0; i<n-1; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    for (int i=1; i<=n; i++) {
        depth[i] = -1;
    }
    queue<int> q;
    q.push(1);
    depth[1] = 0;
    parent0[1] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : graph[u]) {
            if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                parent0[v] = u;
                q.push(v);
            }
        }
    }

    for (int i=1; i<=n; i++) {
        parent_table[0][i] = parent0[i];
    }

    for (int j=1; j<MAX_LOG; j++) {
        for (int i=1; i<=n; i++) {
            int prev = parent_table[j-1][i];
            if (prev == 0) {
                parent_table[j][i] = 0;
            } else {
                parent_table[j][i] = parent_table[j-1][prev];
            }
        }
    }

    auto lca = [&](int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int j=0; j<MAX_LOG; j++) {
            if (diff & (1 << j)) {
                u = parent_table[j][u];
            }
        }
        if (u == v) return u;
        for (int j=MAX_LOG-1; j>=0; j--) {
            if (parent_table[j][u] != parent_table[j][v]) {
                u = parent_table[j][u];
                v = parent_table[j][v];
            }
        }
        return parent_table[0][u];
    };

    auto dist = [&](int u, int v) {
        int w = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[w];
    };

    auto climb = [&](int u, int steps) {
        int node = u;
        for (int j=0; j<MAX_LOG; j++) {
            if (steps & (1 << j)) {
                node = parent_table[j][node];
            }
        }
        return node;
    };

    int Q;
    cin >> Q;

    while (Q--) {
        int k;
        cin >> k;
        vector<int> S(k);
        for (int i=0; i<k; i++) {
            cin >> S[i];
        }

        if (k == 1) {
            cout << "0\n";
            continue;
        }

        int u0 = S[0];
        int u_candidate = u0;
        int max_d = -1;
        for (int i=0; i<k; i++) {
            int d_val = dist(u0, S[i]);
            if (d_val > max_d) {
                max_d = d_val;
                u_candidate = S[i];
            }
        }

        int u_node = u_candidate;
        max_d = -1;
        int v_candidate = u0;
        for (int i=0; i<k; i++) {
            int d_val = dist(u_node, S[i]);
            if (d_val > max_d) {
                max_d = d_val;
                v_candidate = S[i];
            }
        }
        int v_node = v_candidate;

        int w_node = lca(u_node, v_node);
        int d1 = depth[u_node] - depth[w_node];
        int d2 = depth[v_node] - depth[w_node];
        int L = d1 + d2;

        auto get_node_on_path = [&](int t) -> int {
            if (t <= d1) {
                return climb(u_node, t);
            } else {
                return climb(v_node, L - t);
            }
        };

        auto evaluate = [&](int x) -> long long {
            long long total = 0;
            for (int s : S) {
                total += dist(s, x);
            }
            return total;
        };

        long long best = LLONG_MAX;
        int low = 0, high = L;
        while (high - low > 2) {
            int mid1 = low + (high - low) / 3;
            int mid2 = high - (high - low) / 3;
            long long f1 = evaluate(get_node_on_path(mid1));
            long long f2 = evaluate(get_node_on_path(mid2));
            if (f1 < f2) {
                high = mid2;
            } else {
                low = mid1;
            }
        }
        for (int t = low; t <= high; t++) {
            long long val = evaluate(get_node_on_path(t));
            if (val < best) {
                best = val;
            }
        }

        cout << best << '\n';
    }

    return 0;
}