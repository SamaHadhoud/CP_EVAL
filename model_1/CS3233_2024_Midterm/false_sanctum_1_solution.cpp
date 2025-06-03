#include <iostream>
#include <string>
using namespace std;

const int MAXN = 200000;
struct Node {
    int size;
    char lc, rc;
    int lrun, rrun;
    long long dens;
    Node() : size(0), lc(0), rc(0), lrun(0), rrun(0), dens(0) {}
    Node(int sz, char left_char, char right_char, int left_run, int right_run, long long density)
        : size(sz), lc(left_char), rc(right_char), lrun(left_run), rrun(right_run), dens(density) {}
};

Node tree[4 * MAXN];
string s;

Node merge(Node L, Node R) {
    if (L.size == 0) return R;
    if (R.size == 0) return L;
    Node res;
    res.size = L.size + R.size;
    res.lc = L.lc;
    res.rc = R.rc;

    if (L.lrun == L.size && L.rc == R.lc) {
        res.lrun = L.lrun + R.lrun;
    } else {
        res.lrun = L.lrun;
    }

    if (R.rrun == R.size && L.rc == R.lc) {
        res.rrun = R.rrun + L.rrun;
    } else {
        res.rrun = R.rrun;
    }

    res.dens = L.dens + R.dens;
    if (L.rc == R.lc) {
        long long subtract = (long long)L.rrun * L.rrun + (long long)R.lrun * R.lrun;
        long long add = (long long)(L.rrun + R.lrun) * (L.rrun + R.lrun);
        res.dens = res.dens - subtract + add;
    }
    return res;
}

void build(int node, int l, int r) {
    if (l == r) {
        tree[node] = Node(1, s[l], s[l], 1, 1, 1);
        return;
    }
    int mid = (l + r) / 2;
    build(2 * node + 1, l, mid);
    build(2 * node + 2, mid + 1, r);
    tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
}

void update(int node, int l, int r, int idx, char c) {
    if (l == r) {
        s[l] = c;
        tree[node] = Node(1, c, c, 1, 1, 1);
        return;
    }
    int mid = (l + r) / 2;
    if (idx <= mid) {
        update(2 * node + 1, l, mid, idx, c);
    } else {
        update(2 * node + 2, mid + 1, r, idx, c);
    }
    tree[node] = merge(tree[2 * node + 1], tree[2 * node + 2]);
}

Node query(int node, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) {
        return tree[node];
    }
    int mid = (l + r) / 2;
    Node left_node, right_node;
    bool left_used = false, right_used = false;
    if (ql <= mid) {
        left_node = query(2 * node + 1, l, mid, ql, min(qr, mid));
        left_used = true;
    }
    if (qr > mid) {
        right_node = query(2 * node + 2, mid + 1, r, max(ql, mid + 1), qr);
        right_used = true;
    }
    if (left_used && right_used) {
        return merge(left_node, right_node);
    } else if (left_used) {
        return left_node;
    } else if (right_used) {
        return right_node;
    }
    return Node();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;
    cin >> s;

    build(0, 0, N - 1);

    for (int q = 0; q < Q; q++) {
        int type;
        cin >> type;
        if (type == 1) {
            int i;
            char c;
            cin >> i >> c;
            update(0, 0, N - 1, i - 1, c);
        } else {
            int l, r;
            cin >> l >> r;
            Node res = query(0, 0, N - 1, l - 1, r - 1);
            cout << res.dens << '\n';
        }
    }

    return 0;
}