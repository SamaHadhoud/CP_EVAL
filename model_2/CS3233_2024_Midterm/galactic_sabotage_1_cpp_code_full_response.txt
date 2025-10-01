```cpp
#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<int> mn, cnt, lazy;

    SegTree(int n = 0) { init(n); }

    void init(int n_) {
        n = n_;
        mn.assign(4 * n + 4, 0);
        cnt.assign(4 * n + 4, 0);
        lazy.assign(4 * n + 4, 0);
        if (n > 0) build(1, 1, n);
    }

    void build(int idx, int l, int r) {
        lazy[idx] = 0;
        if (l == r) {
            mn[idx] = 0;
            cnt[idx] = 1;
            return;
        }
        int mid = (l + r) >> 1;
        build(idx << 1, l, mid);
        build(idx << 1 | 1, mid + 1, r);
        pull(idx);
    }

    inline void apply(int idx, int delta) {
        mn[idx] += delta;
        lazy[idx] += delta;
    }

    inline void pull(int idx) {
        int L = idx << 1, R = idx << 1 | 1;
        mn[idx] = min(mn[L], mn[R]);
        cnt[idx] = 0;
        if (mn[L] == mn[idx]) cnt[idx] += cnt[L];
        if (mn[R] == mn[idx]) cnt[idx] += cnt[R];
    }

    inline void push(int idx) {
        if (lazy[idx] != 0) {
            int L = idx << 1, R = idx << 1 | 1;
            apply(L, lazy[idx]);
            apply(R, lazy[idx]);
            lazy[idx] = 0;
        }
    }

    void range_add(int idx, int l, int r, int ql, int qr, int delta) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            apply(idx, delta);
            return;
        }
        push(idx);
        int mid = (l + r) >> 1;
        range_add(idx << 1, l, mid, ql, qr, delta);
        range_add(idx << 1 | 1, mid + 1, r, ql, qr, delta);
        pull(idx);
    }

    void range_add(int l, int r, int delta) {
        if (l > r) return;
        range_add(1, 1, n, l, r, delta);
    }

    int get_cnt_min() const {
        return cnt[1];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<int> A(N + 1);
    for (int i = 1; i <= N; ++i) cin >> A[i];

    SegTree st(N);

    // Initialize C by adding +1 to [p..A[p]-1] for every A[p] > p
    for (int p = 1; p <= N; ++p) {
        int v = A[p];
        if (v > p) st.range_add(p, v - 1, +1);
    }

    cout << st.get_cnt_min() << '\n';

    for (int qi = 0; qi < M; ++qi) {
        int u, v;
        cin >> u >> v;
        if (u == v) {
            cout << st.get_cnt_min() << '\n';
            continue;
        }
        int x = A[u], y = A[v];

        // Remove old contributions
        if (x > u) st.range_add(u, x - 1, -1);
        if (y > v) st.range_add(v, y - 1, -1);

        // Swap
        swap(A[u], A[v]);

        // Add new contributions
        if (y > u) st.range_add(u, y - 1, +1);
        if (x > v) st.range_add(v, x - 1, +1);

        cout << st.get_cnt_min() << '\n';
    }

    return 0;
}
```