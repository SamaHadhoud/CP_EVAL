```cpp
#include <bits/stdc++.h>
using namespace std;

struct SegMM {
    int sz;
    vector<long long> mn, mx;
    static constexpr long long INF = (1LL<<62);

    void init(int n) {
        sz = 1;
        while (sz < n) sz <<= 1;
        mn.assign(2*sz, INF);
        mx.assign(2*sz, -INF);
    }
    void build(const vector<long long>& S, int n) {
        init(n);
        for (int i = 1; i <= n; ++i) {
            mn[sz + i - 1] = mx[sz + i - 1] = S[i];
        }
        for (int i = sz - 1; i >= 1; --i) {
            mn[i] = std::min(mn[i<<1], mn[i<<1|1]);
            mx[i] = std::max(mx[i<<1], mx[i<<1|1]);
        }
    }
    inline void update(int pos, long long val) {
        int idx = sz + pos - 1;
        mn[idx] = mx[idx] = val;
        idx >>= 1;
        while (idx) {
            long long a = mn[idx<<1], b = mn[idx<<1|1];
            long long c = mx[idx<<1], d = mx[idx<<1|1];
            mn[idx] = (a < b ? a : b);
            mx[idx] = (c > d ? c : d);
            idx >>= 1;
        }
    }
    inline long long getMin() const { return mn[1]; }
    inline long long getMax() const { return mx[1]; }
};

struct FastScanner {
    static const int BUFSIZE = 1<<20;
    int idx, size;
    char buf[BUFSIZE];
    FastScanner(): idx(0), size(0) {}
    inline char getch() {
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
        c = getch(); if (!c) return false;
        while (c != '-' && (c < '0' || c > '9')) { c = getch(); if (!c) return false; }
        if (c == '-') { sign = -1; c = getch(); }
        for (; c >= '0' && c <= '9'; c = getch()) val = val * 10 + (c - '0');
        out = val * sign;
        return true;
    }
};

static inline long long gcdll(long long a, long long b) {
    while (b) { long long t = a % b; a = b; b = t; }
    return a;
}

static inline void get_divisors_excl1(int n, vector<int>& divs) {
    divs.clear();
    for (int d = 2; (long long)d * d <= n; ++d) {
        if (n % d == 0) {
            divs.push_back(d);
            if (d != n / d) divs.push_back(n / d);
        }
    }
    if (n > 1) divs.push_back(n); // include n itself (k can be n)
    // optional: sort for determinism (not required)
    // sort(divs.begin(), divs.end());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    FastScanner fs;

    int t;
    if (!fs.readInt(t)) return 0;
    while (t--) {
        int n, a1;
        fs.readInt(n);
        fs.readInt(a1);
        vector<int> A(n-1);
        for (int i = 0; i < n-1; ++i) fs.readInt(A[i]);

        vector<int> divs;
        get_divisors_excl1(n, divs);

        long long best_num = (1LL<<62), best_den = 1; // large initial

        vector<long long> base; base.reserve(1024);
        vector<long long> S; S.reserve(1024);
        SegMM seg;

        for (int k : divs) {
            if (k <= 1) continue;

            // baseline sums by residues
            base.assign(k + 1, 0);
            for (int i = 0; i < n-1; ++i) {
                int r = (i % k) + 1;
                base[r] += A[i];
            }

            // initial S for z = 0
            S.assign(k + 1, 0);
            S[1] = base[k] + a1;
            for (int j = 2; j <= k; ++j) S[j] = base[j-1];

            seg.build(S, k);

            for (int z = 0; z < n; ++z) {
                long long m = seg.getMin();
                long long M = seg.getMax();
                __int128 lhs = (__int128)M * best_den;
                __int128 rhs = (__int128)best_num * m;
                if (lhs < rhs) {
                    best_num = M;
                    best_den = m;
                }

                if (z == n-1) break;
                int r = (z % k) + 1;
                int r2 = (r == k ? 1 : r + 1);
                long long v = A[z];
                long long new1 = S[r] + v - a1;
                long long new2 = S[r2] - v + a1;

                S[r] = new1; S[r2] = new2;
                seg.update(r, new1);
                seg.update(r2, new2);
            }
        }

        long long g = gcdll(best_num, best_den);
        best_num /= g; best_den /= g;
        cout << best_num << ' ' << best_den << '\n';
    }
    return 0;
}
```