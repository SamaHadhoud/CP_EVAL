```cpp
#include <bits/stdc++.h>
using namespace std;

struct AC {
    static const int ALPHA = 26;

    struct Node {
        int next[ALPHA];
        Node() { memset(next, -1, sizeof(next)); }
    };

    vector<Node> trie;
    vector<int> fail;

    // failure tree as adjacency list via compressed arrays
    vector<int> head, to, nxt;
    int ecnt = 0;

    // Euler tour
    vector<int> tin, tout;
    int timer = 0;

    AC() {
        trie.reserve(200000 + 5);
        fail.reserve(200000 + 5);
        head.reserve(200000 + 5);
        add_node(); // root = 0
    }

    int add_node() {
        trie.emplace_back();
        fail.push_back(0);
        head.push_back(-1);
        return (int)trie.size() - 1;
    }

    void insert(const string& s) {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'A';
            if (trie[u].next[c] == -1) {
                int v = add_node();
                trie[u].next[c] = v;
            }
            u = trie[u].next[c];
        }
    }

    void build_fail() {
        queue<int> q;
        // Initialize root's children
        for (int c = 0; c < ALPHA; ++c) {
            int v = trie[0].next[c];
            if (v != -1) {
                fail[v] = 0;
                q.push(v);
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int c = 0; c < ALPHA; ++c) {
                int v = trie[u].next[c];
                if (v != -1) {
                    int f = fail[u];
                    while (f && trie[f].next[c] == -1) f = fail[f];
                    if (trie[f].next[c] != -1) f = trie[f].next[c];
                    fail[v] = f;
                    q.push(v);
                }
            }
        }
        // Build failure tree
        int n = (int)trie.size();
        to.clear(); nxt.clear(); ecnt = 0;
        to.reserve(n - 1);
        nxt.reserve(n - 1);
        for (int v = 1; v < n; ++v) {
            add_edge(fail[v], v);
        }
    }

    void add_edge(int u, int v) {
        to.push_back(v);
        nxt.push_back(head[u]);
        head[u] = ecnt++;
    }

    void build_euler() {
        int n = (int)trie.size();
        tin.assign(n, 0);
        tout.assign(n, 0);
        timer = 0;
        vector<int> it(n, -1);
        vector<int> st;
        st.reserve(n);
        st.push_back(0);
        it[0] = -1;
        while (!st.empty()) {
            int u = st.back();
            if (it[u] == -1) {
                it[u] = head[u];
                tin[u] = ++timer;
            }
            int e = it[u];
            if (e == -1) {
                tout[u] = timer + 1; // half-open
                st.pop_back();
                continue;
            }
            it[u] = nxt[e];
            int v = to[e];
            st.push_back(v);
            it[v] = -1;
        }
    }

    inline int go(int state, int c) const { // c in [0..25]
        while (state && trie[state].next[c] == -1) state = fail[state];
        int nx = trie[state].next[c];
        if (nx != -1) state = nx;
        else state = 0;
        return state;
    }

    int size() const { return (int)trie.size(); }
};

struct BIT {
    int n;
    vector<long long> bit;
    BIT() : n(0) {}
    BIT(int n_) { init(n_); }
    void init(int n_) {
        n = n_;
        bit.assign(n + 1, 0);
    }
    void add(int idx, long long val) {
        for (int i = idx; i <= n; i += i & -i) bit[i] += val;
    }
    long long sumPrefix(int idx) const {
        long long res = 0;
        for (int i = idx; i > 0; i -= i & -i) res += bit[i];
        return res;
    }
    long long rangeSum(int l, int r) const {
        if (l > r) return 0;
        if (r < 1) return 0;
        if (l < 1) l = 1;
        if (r > n) r = n;
        return sumPrefix(r) - sumPrefix(l - 1);
    }
};

struct Event {
    int x;      // sweep threshold on x
    int y1, y2; // inclusive y-range
    int id;     // query id
    int coef;   // +1 or -1
    bool operator<(Event const& other) const {
        if (x != other.x) return x < other.x;
        return coef < other.coef; // doesn't really matter
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int R, C, Q;
    if (!(cin >> R >> C >> Q)) return 0;
    vector<string> grid(R);
    for (int r = 0; r < R; ++r) {
        cin >> grid[r];
    }

    vector<string> queries(Q);
    long long totalLen = 0;
    for (int i = 0; i < Q; ++i) {
        cin >> queries[i];
        totalLen += (int)queries[i].size();
    }

    AC acRow, acCol;
    // Insert all queries and their reverses
    for (int i = 0; i < Q; ++i) {
        acRow.insert(queries[i]);
        string t = queries[i];
        reverse(t.begin(), t.end());
        acCol.insert(t);
    }

    acRow.build_fail();
    acRow.build_euler();
    acCol.build_fail();
    acCol.build_euler();

    // Scan grid to compute a[r][c] and b[r][c]
    int M = R * C;
    vector<int> a(M), b(M);

    // Row scan: left-to-right
    for (int r = 0; r < R; ++r) {
        int state = 0;
        for (int c = 0; c < C; ++c) {
            int ch = grid[r][c] - 'A';
            state = acRow.go(state, ch);
            a[r * C + c] = acRow.tin[state];
        }
    }

    // Column scan: bottom-to-top, b[r][c] is state BEFORE reading grid[r][c]
    for (int c = 0; c < C; ++c) {
        int state = 0;
        for (int r = R - 1; r >= 0; --r) {
            b[r * C + c] = acCol.tin[state];
            int ch = grid[r][c] - 'A';
            state = acCol.go(state, ch);
        }
    }

    // Points: (x, y) = (a[r][c], b[r][c])
    struct Point { int x, y; };
    vector<Point> points;
    points.reserve(M);
    for (int idx = 0; idx < M; ++idx) {
        points.push_back({a[idx], b[idx]});
    }
    sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
        if (p1.x != p2.x) return p1.x < p2.x;
        return p1.y < p2.y;
    });

    // Build rectangle events
    vector<Event> events;
    events.reserve((size_t)totalLen * 2 + 10);
    for (int i = 0; i < Q; ++i) {
        const string& s = queries[i];
        int L = (int)s.size();

        // Walk S in acRow to get states for prefixes
        vector<int> vstate(L + 1, 0);
        for (int k = 1; k <= L; ++k) {
            int ch = s[k - 1] - 'A';
            vstate[k] = acRow.go(vstate[k - 1], ch);
        }

        // Walk reverse(S) in acCol to get prefix states
        string t(s.rbegin(), s.rend());
        vector<int> wpref(L + 1, 0);
        for (int pos = 1; pos <= L; ++pos) {
            int ch = t[pos - 1] - 'A';
            wpref[pos] = acCol.go(wpref[pos - 1], ch);
        }

        // For each split k=1..L, pre=S[0..k-1], suf=S[k..L-1]
        for (int k = 1; k <= L; ++k) {
            int v = vstate[k];
            int w = wpref[L - k];
            int x1 = acRow.tin[v];
            int x2 = acRow.tout[v] - 1; // inclusive
            int y1 = acCol.tin[w];
            int y2 = acCol.tout[w] - 1; // inclusive

            // Convert rectangle to two events at x2 (+1) and x1-1 (-1)
            events.push_back({x2, y1, y2, i, +1});
            events.push_back({x1 - 1, y1, y2, i, -1});
        }
    }

    sort(events.begin(), events.end());

    // Sweep line over x with BIT over y
    vector<long long> ans(Q, 0);
    BIT bit(acCol.size()); // y in [1..|acCol|]
    int p = 0;
    for (const auto& e : events) {
        while (p < (int)points.size() && points[p].x <= e.x) {
            bit.add(points[p].y, 1);
            ++p;
        }
        long long cnt = bit.rangeSum(e.y1, e.y2);
        ans[e.id] += (long long)e.coef * cnt;
    }

    // Output
    for (int i = 0; i < Q; ++i) {
        cout << ans[i] << '\n';
    }
    return 0;
}
```