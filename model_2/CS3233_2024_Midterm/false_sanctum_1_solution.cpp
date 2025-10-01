#include <bits/stdc++.h>
using namespace std;

struct Node {
    int len = 0;
    int pref = 0;
    int suff = 0;
    char first = 0;
    char last = 0;
    long long sum = 0;
};

int N, Q;
vector<char> S;
vector<Node> st;

inline Node make_leaf(char c) {
    Node x;
    x.len = 1;
    x.pref = 1;
    x.suff = 1;
    x.first = c;
    x.last = c;
    x.sum = 1;
    return x;
}

inline Node merge_nodes(const Node& A, const Node& B) {
    if (A.len == 0) return B;
    if (B.len == 0) return A;
    Node C;
    C.len = A.len + B.len;
    C.first = A.first;
    C.last = B.last;

    C.sum = A.sum + B.sum;
    if (A.last == B.first) {
        long long a = A.suff;
        long long b = B.pref;
        C.sum += (a + b) * (a + b) - a * a - b * b;
    }

    C.pref = A.pref;
    if (A.pref == A.len && A.last == B.first) {
        C.pref = A.len + B.pref;
    }

    C.suff = B.suff;
    if (B.suff == B.len && A.last == B.first) {
        C.suff = B.len + A.suff;
    }

    return C;
}

void build(int p, int L, int R) {
    if (L == R) {
        st[p] = make_leaf(S[L]);
        return;
    }
    int M = (L + R) >> 1;
    build(p << 1, L, M);
    build(p << 1 | 1, M + 1, R);
    st[p] = merge_nodes(st[p << 1], st[p << 1 | 1]);
}

void update(int p, int L, int R, int idx, char c) {
    if (L == R) {
        st[p] = make_leaf(c);
        return;
    }
    int M = (L + R) >> 1;
    if (idx <= M) update(p << 1, L, M, idx, c);
    else update(p << 1 | 1, M + 1, R, idx, c);
    st[p] = merge_nodes(st[p << 1], st[p << 1 | 1]);
}

Node query(int p, int L, int R, int ql, int qr) {
    if (ql <= L && R <= qr) return st[p];
    int M = (L + R) >> 1;
    if (qr <= M) return query(p << 1, L, M, ql, qr);
    if (ql > M) return query(p << 1 | 1, M + 1, R, ql, qr);
    Node left = query(p << 1, L, M, ql, qr);
    Node right = query(p << 1 | 1, M + 1, R, ql, qr);
    return merge_nodes(left, right);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> Q;
    string s0;
    cin >> s0;

    S.assign(N + 1, 0);
    for (int i = 1; i <= N; ++i) S[i] = s0[i - 1];

    st.assign(4 * N + 5, Node());
    build(1, 1, N);

    for (int qi = 0; qi < Q; ++qi) {
        int type;
        cin >> type;
        if (type == 1) {
            int i;
            char c;
            cin >> i >> c;
            if (S[i] != c) {
                S[i] = c;
                update(1, 1, N, i, c);
            }
        } else { // type == 2
            int l, r;
            cin >> l >> r;
            Node ans = query(1, 1, N, l, r);
            cout << ans.sum << '\n';
        }
    }

    return 0;
}