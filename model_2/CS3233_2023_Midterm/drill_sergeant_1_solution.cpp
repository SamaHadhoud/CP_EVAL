#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using u64 = unsigned long long;

struct Solver {
    unordered_set<u64> H; // dislike pairs encoded as 64-bit (min<<32 | max)
    set<int> S;           // ordered set of cats currently in line
    int64 total = 0;

    static inline u64 key(int a, int b) {
        if (a > b) swap(a, b);
        return (u64(uint32_t)a << 32) | uint32_t(b);
    }

    inline bool dislike(int a, int b) const {
        // assumes a,b in [1..n], but safe even if not
        if (a <= 0 || b <= 0) return false;
        return H.find(key(a, b)) != H.end();
    }

    int value(int x) const {
        auto it = S.find(x);
        bool F = false, B = false;
        if (it != S.begin()) {
            int p = *prev(it);
            F = dislike(p, x);
        }
        auto jt = next(it);
        if (jt != S.end()) {
            int s = *jt;
            B = dislike(x, s);
        }
        if (F && B) return 3233;
        if (F) return 323;
        if (B) return 32;
        return 3;
    }

    void insert_cat(int z) {
        // find predecessor and successor before insertion
        auto it = S.lower_bound(z);
        int s = (it == S.end() ? 0 : *it);
        int p = (it == S.begin() ? 0 : *prev(it));

        if (p) total -= value(p);
        if (s) total -= value(s);

        S.insert(z);

        total += value(z);
        if (p) total += value(p);
        if (s) total += value(s);
    }

    void erase_cat(int z) {
        auto it = S.find(z);
        int p = (it == S.begin() ? 0 : *prev(it));
        int s = (next(it) == S.end() ? 0 : *next(it));

        total -= value(z);
        if (p) total -= value(p);
        if (s) total -= value(s);

        S.erase(it);

        if (p) total += value(p);
        if (s) total += value(s);
    }

    void run() {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        int n, m;
        if (!(cin >> n >> m)) return;

        H.reserve((size_t)max(1, m * 2));
        H.max_load_factor(0.7f);

        for (int i = 0; i < m; ++i) {
            int x, y;
            cin >> x >> y;
            H.insert(key(x, y));
        }

        int q; cin >> q;
        while (q--) {
            int d, z;
            cin >> d >> z;
            if (d == 1) {
                insert_cat(z);
            } else {
                erase_cat(z);
            }
            cout << total << '\n';
        }
    }
};

int main() {
    Solver().run();
    return 0;
}