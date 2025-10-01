#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<long long> bit;
    Fenwick() {}
    Fenwick(int n): n(n), bit(n+2, 0) {}
    void reset() { fill(bit.begin(), bit.end(), 0); }
    void add(int idx, long long val) {
        for (; idx <= n+1; idx += idx & -idx) bit[idx] += val;
    }
    void rangeAdd(int l, int r, long long val) {
        if (l > r) return;
        add(l, val);
        add(r+1, -val);
    }
    long long pointQuery(int idx) {
        long long s = 0;
        for (; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }
};

struct PST {
    struct Node { int l, r, sum; };
    int N;
    vector<Node> t;
    PST() {}
    PST(int n) : N(n) {
        t.reserve(4000000); // reserve generously
        t.push_back({0,0,0}); // index 0 is null node
    }
    int newNode(const Node &from) {
        t.push_back(from);
        return (int)t.size() - 1;
    }
    int update(int prev, int L, int R, int pos) {
        int cur = newNode(t[prev]);
        t[cur].sum = t[prev].sum + 1;
        if (L == R) return cur;
        int M = (L + R) >> 1;
        if (pos <= M) {
            int nl = update(t[prev].l, L, M, pos);
            t[cur].l = nl;
        } else {
            int nr = update(t[prev].r, M+1, R, pos);
            t[cur].r = nr;
        }
        return cur;
    }
    // kth in (r1 - l1) - (r2 - l2)
    int kth(int r1, int l1, int r2, int l2, int L, int R, int k) {
        if (L == R) return L;
        int M = (L + R) >> 1;
        int rl1 = t[r1].l, ll1 = t[l1].l, rl2 = t[r2].l, ll2 = t[l2].l;
        int leftCount = (t[rl1].sum - t[ll1].sum) - (t[rl2].sum - t[ll2].sum);
        if (k <= leftCount) {
            return kth(rl1, ll1, rl2, ll2, L, M, k);
        } else {
            int rr1 = t[r1].r, lr1 = t[l1].r, rr2 = t[r2].r, lr2 = t[l2].r;
            return kth(rr1, lr1, rr2, lr2, M+1, R, k - leftCount);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, q;
    if (!(cin >> n >> q)) return 0;
    vector<int> p(n+1);
    int root = 0;
    for (int i = 1; i <= n; ++i) {
        cin >> p[i];
        if (p[i] == 0) root = i;
    }
    vector<vector<int>> children(n+1);
    for (int i = 1; i <= n; ++i) {
        if (p[i] != 0) children[p[i]].push_back(i);
    }

    // Euler tour (iterative), depth, tin/tout
    vector<int> tin(n+1), tout(n+1), euler(n+1), depth(n+1), parent(n+1);
    parent = p;
    int timer = 0;
    {
        vector<pair<int,int>> st;
        st.reserve(2*n);
        st.emplace_back(root, 0); // 0=enter, 1=exit
        depth[root] = 0;
        while (!st.empty()) {
            auto [u, state] = st.back(); st.pop_back();
            if (state == 0) {
                tin[u] = ++timer;
                euler[timer] = u;
                st.emplace_back(u, 1);
                // push children in reverse to process in original order if needed
                for (int i = (int)children[u].size() - 1; i >= 0; --i) {
                    int v = children[u][i];
                    depth[v] = depth[u] + 1;
                    parent[v] = u;
                    st.emplace_back(v, 0);
                }
            } else {
                tout[u] = timer;
            }
        }
    }
    vector<int> sz(n+1);
    for (int u = 1; u <= n; ++u) {
        sz[u] = tout[u] - tin[u] + 1;
    }

    // Binary lifting
    int LOG = 1;
    while ((1 << LOG) <= n) ++LOG;
    vector<vector<int>> up(LOG, vector<int>(n+1, 0));
    for (int u = 1; u <= n; ++u) up[0][u] = parent[u];
    for (int j = 1; j < LOG; ++j) {
        for (int u = 1; u <= n; ++u) {
            int mid = up[j-1][u];
            up[j][u] = (mid ? up[j-1][mid] : 0);
        }
    }
    auto jump_up = [&](int u, int steps) {
        for (int j = 0; j < LOG && u; ++j) {
            if (steps & (1 << j)) u = up[j][u];
        }
        return u;
    };

    // Persistent Segment Tree (Chairman Tree)
    PST pst(n);
    vector<int> roots(n+1, 0); // roots[i] = version after processing euler[1..i]
    for (int i = 1; i <= n; ++i) {
        int v = euler[i];
        roots[i] = pst.update(roots[i-1], 1, n, v);
    }

    // Read queries, precompute x,t
    struct Query { long long k; int x, t; int l; int b; };
    vector<Query> qs(q);
    for (int i = 0; i < q; ++i) {
        long long k; cin >> k;
        int x = (int)((k - 1) / n) + 1;
        int t = (int)((k - 1) % n) + 1;
        qs[i] = {k, x, t, 0, 0};
    }

    // Parallel Binary Search to find l for each query
    vector<int> low(q, 1), high(q, n);
    Fenwick fw(n);
    vector<vector<int>> buckets(n+2);
    while (true) {
        bool any = false;
        for (int i = 1; i <= n; ++i) buckets[i].clear();
        for (int i = 0; i < q; ++i) {
            if (low[i] < high[i]) {
                any = true;
                int mid = (low[i] + high[i]) >> 1;
                buckets[mid].push_back(i);
            }
        }
        if (!any) break;

        fw.reset();
        for (int l = 1; l <= n; ++l) {
            // Apply events for label l
            fw.rangeAdd(tin[l], tout[l], sz[l]); // Type A
            for (int u : children[l]) {
                fw.rangeAdd(tin[u], tout[u], -sz[u]); // Type B for each child
            }
            // Answer buckets at l
            for (int idx : buckets[l]) {
                int x = qs[idx].x;
                long long val = fw.pointQuery(tin[x]);
                if (val >= qs[idx].t) high[idx] = l;
                else low[idx] = l + 1;
            }
        }
    }
    for (int i = 0; i < q; ++i) qs[i].l = low[i];

    // Compute b = t - P_x(l-1)
    vector<vector<int>> group(n+2);
    for (int i = 1; i <= n; ++i) group[i].clear();
    for (int i = 0; i < q; ++i) group[qs[i].l].push_back(i);

    fw.reset();
    for (int l = 1; l <= n; ++l) {
        for (int idx : group[l]) {
            int x = qs[idx].x;
            long long pref = fw.pointQuery(tin[x]); // P_x(l-1)
            qs[idx].b = qs[idx].t - (int)pref;
        }
        fw.rangeAdd(tin[l], tout[l], sz[l]);
        for (int u : children[l]) {
            fw.rangeAdd(tin[u], tout[u], -sz[u]);
        }
    }

    // Answer queries: select y and compute final value
    vector<long long> answers(q);
    for (int i = 0; i < q; ++i) {
        int x = qs[i].x;
        int l = qs[i].l;
        int b = qs[i].b;

        int a1 = 0;
        if (l != x) {
            int dist = depth[x] - depth[l] - 1;
            a1 = jump_up(x, dist);
        }
        int r1 = roots[tout[l]], l1 = roots[tin[l]-1];
        int r2 = 0, l2 = 0;
        if (a1) { r2 = roots[tout[a1]]; l2 = roots[tin[a1]-1]; }
        int y = pst.kth(r1, l1, r2, l2, 1, n, b);

        long long nn = (long long)n;
        long long ans = (long long)(x - 1) * nn * nn + (long long)(l - 1) * nn + (long long)(y - 1);
        answers[i] = ans;
    }

    for (int i = 0; i < q; ++i) {
        cout << answers[i] << '\n';
    }
    return 0;
}