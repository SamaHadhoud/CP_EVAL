#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
typedef long long ll;

class SegmentTree {
public:
    vector<ll> tree;
    vector<ll> lazy;
    int n;
    int size;

    SegmentTree(vector<ll>& data, int n_val) {
        n = n_val;
        size = 1;
        while (size < n) size *= 2;
        tree.assign(2 * size, (ll)1e18);
        lazy.assign(2 * size, 0);
        for (int i = 0; i < n; i++) {
            tree[size + i] = data[i];
        }
        for (int i = size - 1; i >= 1; i--) {
            tree[i] = min(tree[2 * i], tree[2 * i + 1]);
        }
    }

    void apply(int node, ll delta) {
        tree[node] += delta;
        if (node < size) {
            lazy[node] += delta;
        }
    }

    void push(int node) {
        if (lazy[node] != 0) {
            apply(2 * node, lazy[node]);
            apply(2 * node + 1, lazy[node]);
            lazy[node] = 0;
        }
    }

    void update_range(int l, int r, ll delta, int node, int segL, int segR) {
        if (r < segL || segR < l) return;
        if (l <= segL && segR <= r) {
            apply(node, delta);
            return;
        }
        push(node);
        int mid = (segL + segR) / 2;
        update_range(l, r, delta, 2 * node, segL, mid);
        update_range(l, r, delta, 2 * node + 1, mid + 1, segR);
        tree[node] = min(tree[2 * node], tree[2 * node + 1]);
    }

    void update(int l, int r, ll delta) {
        if (l > r) return;
        update_range(l, r, delta, 1, 0, size - 1);
    }

    ll global_min() {
        return tree[1];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, q;
    cin >> n >> m >> q;
    vector<ll> A(n);
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    ll A0 = A[0];
    int F1 = 0;
    for (int i = 0; i < n; i++) {
        if (A[i] < A0) {
            F1++;
        }
    }

    vector<int> R_arr(m + 1);
    vector<vector<ll>> B_arr(m + 1);
    vector<int> G_current(m + 1, 0);

    for (int i = 1; i <= m; i++) {
        cin >> R_arr[i];
        B_arr[i].resize(R_arr[i]);
        for (int j = 0; j < R_arr[i]; j++) {
            cin >> B_arr[i][j];
            if (B_arr[i][j] < A0) {
                G_current[i]++;
            }
        }
    }

    vector<ll> D_arr;
    if (m > 0) {
        D_arr.resize(m);
        ll S = F1;
        for (int i = 1; i <= m; i++) {
            D_arr[i - 1] = S - R_arr[i];
            S = S - R_arr[i] + G_current[i];
        }
    }

    SegmentTree* seg_tree = nullptr;
    if (m > 0) {
        seg_tree = new SegmentTree(D_arr, m);
    }

    vector<int> out_lines;
    for (int idx = 0; idx < q; idx++) {
        int X, Y;
        ll Z;
        cin >> X >> Y >> Z;
        Y--;

        ll old_val = B_arr[X][Y];
        int old_contrib = (old_val < A0) ? 1 : 0;
        int new_contrib = (Z < A0) ? 1 : 0;
        int delta = new_contrib - old_contrib;

        if (delta != 0) {
            G_current[X] += delta;
            if (X < m && seg_tree != nullptr) {
                seg_tree->update(X, m - 1, delta);
            }
            B_arr[X][Y] = Z;
        }

        if (m == 0) {
            out_lines.push_back(1);
        } else {
            ll min_val = seg_tree->global_min();
            if (min_val >= 0) {
                out_lines.push_back(1);
            } else {
                out_lines.push_back(0);
            }
        }
    }

    for (int i = 0; i < out_lines.size(); i++) {
        cout << out_lines[i] << '\n';
    }

    if (seg_tree != nullptr) {
        delete seg_tree;
    }

    return 0;
}