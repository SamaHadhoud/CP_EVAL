```cpp
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n=0): n(n), bit(n+1, 0) {}
    void init(int n_) { n = n_; bit.assign(n+1, 0); }
    void add(int idx, int delta) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }
    int sumPrefix(int idx) const {
        int s = 0;
        for (; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }
    int sumRange(int l, int r) const {
        if (r < l) return 0;
        return sumPrefix(r) - sumPrefix(l-1);
    }
};

static long long ceil_div(long long a, long long b) {
    if (b < 0) a = -a, b = -b;
    if (a >= 0) return (a + b - 1) / b;
    else return a / b; // integer division truncates toward zero -> acts as ceil for a<0, b>0
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    if (!(cin >> N >> K)) return 0;
    vector<int> P(N+1);
    for (int i = 1; i <= N; ++i) cin >> P[i];

    vector<vector<pair<int,int>>> byR(N+1);
    for (int i = 0; i < K; ++i) {
        int A, B, C;
        cin >> A >> B >> C;
        long long L = (long long)B - A + 1;
        long long Tll = ceil_div((long long)C + L, 2);
        if (Tll <= 0) continue;
        if (Tll > L) {
            cout << "Impossible\n";
            return 0;
        }
        byR[B].push_back({A, (int)Tll});
    }

    Fenwick bit(N);
    set<int> S; // free positions (Pi = 0)
    for (int i = 1; i <= N; ++i) {
        if (P[i] == 1) bit.add(i, 1);
        else if (P[i] == 0) S.insert(i);
    }

    vector<char> chosen(N+1, 0);

    for (int R = 1; R <= N; ++R) {
        for (auto &ct : byR[R]) {
            int A = ct.first, T = ct.second;
            int current = bit.sumRange(A, R);
            int need = T - current;
            while (need > 0) {
                auto it = S.upper_bound(R);
                if (it == S.begin()) {
                    cout << "Impossible\n";
                    return 0;
                }
                --it;
                int j = *it;
                if (j < A) {
                    cout << "Impossible\n";
                    return 0;
                }
                S.erase(it);
                bit.add(j, 1);
                chosen[j] = 1;
                --need;
            }
        }
    }

    // Build and print X: Xi = 2*Yi - 1
    // Yi = 1 at fixed ones or chosen ones; otherwise 0.
    for (int i = 1; i <= N; ++i) {
        int Xi;
        if (P[i] == 1) Xi = 1;
        else if (P[i] == -1) Xi = -1;
        else Xi = chosen[i] ? 1 : -1;
        if (i > 1) cout << ' ';
        cout << Xi;
    }
    cout << '\n';
    return 0;
}
```