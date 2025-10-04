```cpp
#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 1000003233LL;

inline long long addmod(long long a, long long b) {
    a += b;
    if (a >= MOD) a -= MOD;
    return a;
}
inline long long submod(long long a, long long b) {
    a -= b;
    if (a < 0) a += MOD;
    return a;
}
inline long long mulmod(long long a, long long b) {
    return (long long)((__int128)a * b % MOD);
}

int n;
vector<long long> A;

// Work arrays reused at each merge step (indexed by original indices)
vector<long long> LminArr, LmaxArr, RminArr, RmaxArr, Pmin, Pmax, Pprod;

long long solve(int l, int r) {
    if (l == r) {
        return mulmod(A[l] % MOD, A[l] % MOD);
    }
    int mid = (l + r) >> 1;
    long long ans = solve(l, mid);
    ans = addmod(ans, solve(mid + 1, r));

    // Build left prefix min/max (towards mid from left)
    LminArr[mid] = A[mid];
    LmaxArr[mid] = A[mid];
    for (int i = mid - 1; i >= l; --i) {
        LminArr[i] = std::min(LminArr[i + 1], A[i]);
        LmaxArr[i] = std::max(LmaxArr[i + 1], A[i]);
    }

    // Build right prefix min/max (from mid+1 to right)
    RminArr[mid + 1] = A[mid + 1];
    RmaxArr[mid + 1] = A[mid + 1];
    for (int j = mid + 2; j <= r; ++j) {
        RminArr[j] = std::min(RminArr[j - 1], A[j]);
        RmaxArr[j] = std::max(RmaxArr[j - 1], A[j]);
    }

    // Prefix sums on the right side
    Pmin[mid] = 0;
    Pmax[mid] = 0;
    Pprod[mid] = 0;
    for (int j = mid + 1; j <= r; ++j) {
        Pmin[j] = addmod(Pmin[j - 1], RminArr[j] % MOD);
        Pmax[j] = addmod(Pmax[j - 1], RmaxArr[j] % MOD);
        long long prod = mulmod(RminArr[j] % MOD, RmaxArr[j] % MOD);
        Pprod[j] = addmod(Pprod[j - 1], prod);
    }

    // Two pointers for thresholds
    int j1 = mid, j2 = mid;
    for (int i = mid; i >= l; --i) {
        // t1: largest j with Rmin[j] >= Lmin[i]
        while (j1 < r && RminArr[j1 + 1] >= LminArr[i]) ++j1;
        // t2: largest j with Rmax[j] <= Lmax[i]
        while (j2 < r && RmaxArr[j2 + 1] <= LmaxArr[i]) ++j2;

        int jA = (j1 < j2 ? j1 : j2);

        // Region A: constant Lmin[i]*Lmax[i] over count (jA - mid)
        long long cntA = jA - mid;
        if (cntA > 0) {
            long long constProd = mulmod(LminArr[i] % MOD, LmaxArr[i] % MOD);
            ans = addmod(ans, mulmod(constProd, cntA % MOD));
        }

        // Region B: if j2 > j1 -> Lmax[i] * sum Rmin over (j1, j2]
        if (j2 > j1) {
            long long sumRmin = submod(Pmin[j2], Pmin[j1]);
            ans = addmod(ans, mulmod(LmaxArr[i] % MOD, sumRmin));
        }

        // Region C: if j1 > j2 -> Lmin[i] * sum Rmax over (j2, j1]
        if (j1 > j2) {
            long long sumRmax = submod(Pmax[j1], Pmax[j2]);
            ans = addmod(ans, mulmod(LminArr[i] % MOD, sumRmax));
        }

        // Region D: sum of Rmin*Rmax over (max(j1, j2), r]
        int jm = (j1 > j2 ? j1 : j2);
        long long sumProdTail = submod(Pprod[r], Pprod[jm]);
        ans = addmod(ans, sumProdTail);
    }

    return ans;
}

struct FastScanner {
    static inline int gc() {
        return getchar_unlocked();
    }
    template <class T>
    bool readInt(T &out) {
        int c = gc();
        if (c == EOF) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = gc();
            if (c == EOF) return false;
        }
        long long sign = 1;
        if (c == '-') {
            sign = -1;
            c = gc();
        }
        long long val = 0;
        for (; c >= '0' && c <= '9'; c = gc()) {
            val = val * 10 + (c - '0');
        }
        out = (T)(val * sign);
        return true;
    }
} In;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Use fast scanner for reliability with 1e6 chars input size
    if (!In.readInt(n)) return 0;
    A.assign(n + 2, 0);
    for (int i = 1; i <= n; ++i) {
        In.readInt(A[i]);
    }

    LminArr.assign(n + 2, 0);
    LmaxArr.assign(n + 2, 0);
    RminArr.assign(n + 2, 0);
    RmaxArr.assign(n + 2, 0);
    Pmin.assign(n + 2, 0);
    Pmax.assign(n + 2, 0);
    Pprod.assign(n + 2, 0);

    long long ans = solve(1, n);
    ans %= MOD;
    if (ans < 0) ans += MOD;
    cout << ans << '\n';
    return 0;
}
```