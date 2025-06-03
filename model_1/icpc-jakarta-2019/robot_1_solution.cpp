#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const long long mod = 1000000007;

struct Matrix {
    long long a, b, c, d;
    Matrix() : a(1), b(0), c(0), d(1) {}
    Matrix(long long a0, long long b0, long long c0, long long d0) 
        : a(a0), b(b0), c(c0), d(d0) {}
};

Matrix multiply(const Matrix &m1, const Matrix &m2) {
    return Matrix(
        (m1.a * m2.a % mod + m1.b * m2.c % mod) % mod,
        (m1.a * m2.b % mod + m1.b * m2.d % mod) % mod,
        (m1.c * m2.a % mod + m1.d * m2.c % mod) % mod,
        (m1.c * m2.b % mod + m1.d * m2.d % mod) % mod
    );
}

class SegmentTree {
public:
    vector<Matrix> tree;
    vector<int> lazy;
    int n;

    SegmentTree(string &str) {
        n = str.size();
        tree.resize(4 * n);
        lazy.assign(4 * n, 0);
        build(0, 0, n-1, str);
    }

    void build(int idx, int l, int r, string &s) {
        lazy[idx] = 0;
        if (l == r) {
            if (s[l] == 'A') {
                tree[idx] = Matrix(1, 1, 0, 1);
            } else {
                tree[idx] = Matrix(1, 0, 1, 1);
            }
            return;
        }
        int mid = (l + r) / 2;
        build(2*idx+1, l, mid, s);
        build(2*idx+2, mid+1, r, s);
        tree[idx] = multiply(tree[2*idx+2], tree[2*idx+1]);
    }

    void push(int idx, int l, int r) {
        if (lazy[idx]) {
            swap(tree[idx].a, tree[idx].d);
            swap(tree[idx].b, tree[idx].c);
            if (l != r) {
                lazy[2*idx+1] ^= 1;
                lazy[2*idx+2] ^= 1;
            }
            lazy[idx] = 0;
        }
    }

    void update(int idx, int l, int r, int ql, int qr) {
        push(idx, l, r);
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            swap(tree[idx].a, tree[idx].d);
            swap(tree[idx].b, tree[idx].c);
            if (l != r) {
                lazy[2*idx+1] ^= 1;
                lazy[2*idx+2] ^= 1;
            }
            return;
        }
        int mid = (l + r) / 2;
        update(2*idx+1, l, mid, ql, qr);
        update(2*idx+2, mid+1, r, ql, qr);
        tree[idx] = multiply(tree[2*idx+2], tree[2*idx+1]);
    }

    Matrix query(int idx, int l, int r, int ql, int qr) {
        push(idx, l, r);
        if (qr < l || r < ql) {
            return Matrix(1, 0, 0, 1);
        }
        if (ql <= l && r <= qr) {
            return tree[idx];
        }
        int mid = (l + r) / 2;
        Matrix left_mat = query(2*idx+1, l, mid, ql, qr);
        Matrix right_mat = query(2*idx+2, mid+1, r, ql, qr);
        if (qr < l || r < ql) {
            return Matrix(1, 0, 0, 1);
        }
        if (ql > mid) {
            return right_mat;
        }
        if (qr <= mid) {
            return left_mat;
        }
        return multiply(right_mat, left_mat);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, Q;
    cin >> N >> Q;
    string S;
    cin >> S;

    SegmentTree st(S);

    for (int i = 0; i < Q; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int L, R;
            cin >> L >> R;
            st.update(0, 0, st.n-1, L-1, R-1);
        } else {
            int L, R;
            long long A_val, B_val;
            cin >> L >> R >> A_val >> B_val;
            Matrix M = st.query(0, 0, st.n-1, L-1, R-1);
            long long A_final = (A_val * M.a % mod + B_val * M.b % mod) % mod;
            long long B_final = (A_val * M.c % mod + B_val * M.d % mod) % mod;
            cout << A_final << " " << B_final << "\n";
        }
    }

    return 0;
}