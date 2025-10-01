#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<int> mx, lz;
    SegTree() {}
    SegTree(int n_) { init(n_); }
    void init(int n_) {
        n = n_;
        mx.assign(4*n + 4, 0);
        lz.assign(4*n + 4, 0);
    }
    void build(int idx, int l, int r, const vector<int>& a) {
        if (l == r) {
            mx[idx] = a[l];
            return;
        }
        int m = (l + r) >> 1;
        build(idx<<1, l, m, a);
        build(idx<<1|1, m+1, r, a);
        mx[idx] = max(mx[idx<<1], mx[idx<<1|1]);
    }
    void push(int idx) {
        if (lz[idx] != 0) {
            int v = lz[idx];
            mx[idx<<1] += v; lz[idx<<1] += v;
            mx[idx<<1|1] += v; lz[idx<<1|1] += v;
            lz[idx] = 0;
        }
    }
    void range_add(int idx, int l, int r, int ql, int qr, int v) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            mx[idx] += v;
            lz[idx] += v;
            return;
        }
        push(idx);
        int m = (l + r) >> 1;
        range_add(idx<<1, l, m, ql, qr, v);
        range_add(idx<<1|1, m+1, r, ql, qr, v);
        mx[idx] = max(mx[idx<<1], mx[idx<<1|1]);
    }
    void range_add(int l, int r, int v) {
        if (l > r) return;
        range_add(1, 1, n, l, r, v);
    }
    int get_max() const { return mx[1]; }
};

struct FastScanner {
    static const int BUFSIZE = 1 << 20;
    int idx, size;
    char buf[BUFSIZE];
    FastScanner(): idx(0), size(0) {}
    inline char getChar() {
        if (idx >= size) {
            size = (int)fread(buf, 1, BUFSIZE, stdin);
            idx = 0;
            if (size == 0) return 0;
        }
        return buf[idx++];
    }
    template<typename T>
    bool readInt(T &out) {
        char c; T sign = 1; T val = 0;
        c = getChar();
        if (!c) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = getChar();
            if (!c) return false;
        }
        if (c == '-') { sign = -1; c = getChar(); }
        for (; c >= '0' && c <= '9'; c = getChar()) val = val * 10 + (c - '0');
        out = val * sign;
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    FastScanner fs;
    int N, M, Q;
    if (!fs.readInt(N)) return 0;
    fs.readInt(M);
    fs.readInt(Q);

    long long A1;
    fs.readInt(A1);
    int x0 = 0;
    for (int i = 2; i <= N; ++i) {
        long long v; fs.readInt(v);
        if (v < A1) ++x0;
    }

    vector<vector<unsigned char>> lessFlag(M + 1);
    vector<int> need(M + 1, 0);

    long long prefix = 0; // P_{i-1}
    for (int i = 1; i <= M; ++i) {
        int Ri; fs.readInt(Ri);
        lessFlag[i].assign(Ri, 0);
        int s = 0;
        for (int j = 0; j < Ri; ++j) {
            long long b; fs.readInt(b);
            unsigned char isLess = (b < A1) ? 1 : 0;
            lessFlag[i][j] = isLess;
            s += isLess;
        }
        need[i] = (int)(Ri - prefix);
        prefix += (long long)s - Ri; // t_i = s - R_i
    }

    SegTree st(M);
    st.build(1, 1, M, need);

    ostringstream out;
    for (int qi = 0; qi < Q; ++qi) {
        int X, Y;
        long long Z;
        fs.readInt(X);
        fs.readInt(Y);
        fs.readInt(Z);
        unsigned char newLess = (Z < A1) ? 1 : 0;
        unsigned char &oldLess = lessFlag[X][Y - 1];
        if (newLess != oldLess) {
            int d = (int)newLess - (int)oldLess; // ±1
            oldLess = newLess;
            if (X < M) st.range_add(X + 1, M, -d);
        }
        int H = st.get_max();
        out << (x0 >= H ? 1 : 0) << '\n';
    }

    cout << out.str();
    return 0;
}