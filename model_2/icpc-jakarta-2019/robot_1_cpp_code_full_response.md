```cpp
#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int64 MOD = 1000000007LL;

struct Mat {
    int64 a00, a01, a10, a11;
};

inline Mat mul(const Mat& X, const Mat& Y) {
    Mat Z;
    Z.a00 = (X.a00 * Y.a00 + X.a01 * Y.a10) % MOD;
    Z.a01 = (X.a00 * Y.a01 + X.a01 * Y.a11) % MOD;
    Z.a10 = (X.a10 * Y.a00 + X.a11 * Y.a10) % MOD;
    Z.a11 = (X.a10 * Y.a01 + X.a11 * Y.a11) % MOD;
    return Z;
}

inline Mat toggleMat(const Mat& M) { // P * M * P
    return Mat{M.a11, M.a10, M.a01, M.a00};
}

const Mat Iden{1, 0, 0, 1};
const Mat MA{1, 1, 0, 1};
const Mat MB{1, 0, 1, 1};

int N, Q;
string S;
vector<Mat> seg;
vector<unsigned char> lazyf;

inline void applyFlip(int idx) {
    seg[idx] = toggleMat(seg[idx]);
    lazyf[idx] ^= 1;
}

void build(int idx, int l, int r) {
    if (l == r) {
        seg[idx] = (S[l] == 'A') ? MA : MB;
        lazyf[idx] = 0;
        return;
    }
    int mid = (l + r) >> 1;
    build(idx << 1, l, mid);
    build(idx << 1 | 1, mid + 1, r);
    seg[idx] = mul(seg[idx << 1 | 1], seg[idx << 1]);
    lazyf[idx] = 0;
}

inline void push(int idx) {
    if (lazyf[idx]) {
        applyFlip(idx << 1);
        applyFlip(idx << 1 | 1);
        lazyf[idx] = 0;
    }
}

void updateToggle(int idx, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        applyFlip(idx);
        return;
    }
    push(idx);
    int mid = (l + r) >> 1;
    updateToggle(idx << 1, l, mid, ql, qr);
    updateToggle(idx << 1 | 1, mid + 1, r, ql, qr);
    seg[idx] = mul(seg[idx << 1 | 1], seg[idx << 1]);
}

Mat query(int idx, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return Iden;
    if (ql <= l && r <= qr) return seg[idx];
    push(idx);
    int mid = (l + r) >> 1;
    Mat L = query(idx << 1, l, mid, ql, qr);
    Mat R = query(idx << 1 | 1, mid + 1, r, ql, qr);
    return mul(R, L);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> Q;
    cin >> S;
    S = " " + S; // 1-index
    seg.assign(4 * N + 5, Iden);
    lazyf.assign(4 * N + 5, 0);

    build(1, 1, N);

    for (int qi = 0; qi < Q; ++qi) {
        int type;
        cin >> type;
        if (type == 1) {
            int L, R;
            cin >> L >> R;
            updateToggle(1, 1, N, L, R);
        } else {
            int L, R;
            long long A, B;
            cin >> L >> R >> A >> B;
            Mat M = query(1, 1, N, L, R);
            A %= MOD;
            B %= MOD;
            long long A2 = (M.a00 * A + M.a01 * B) % MOD;
            long long B2 = (M.a10 * A + M.a11 * B) % MOD;
            cout << A2 << ' ' << B2 << '\n';
        }
    }
    return 0;
}
```