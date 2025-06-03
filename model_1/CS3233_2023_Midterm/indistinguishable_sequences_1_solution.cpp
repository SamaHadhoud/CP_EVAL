#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const long long MOD = 1000003233;
const int MAXN = 100100;

class FenwMax {
public:
    vector<int> fenw;
    int n;
    FenwMax(int size) {
        n = size;
        fenw.assign(n+1, 0);
    }

    void update(int idx, int val) {
        while (idx <= n) {
            if (val > fenw[idx]) {
                fenw[idx] = val;
            }
            idx += (idx & -idx);
        }
    }

    int query(int idx) {
        if (idx <= 0) return 0;
        int res = 0;
        while (idx > 0) {
            if (fenw[idx] > res) 
                res = fenw[idx];
            idx -= (idx & -idx);
        }
        return res;
    }
};

class FenwSum {
public:
    vector<long long> fenw;
    int n;
    FenwSum(int size) {
        n = size;
        fenw.assign(n+1, 0);
    }

    void update(int idx, long long delta) {
        while (idx <= n) {
            fenw[idx] = (fenw[idx] + delta) % MOD;
            idx += (idx & -idx);
        }
    }

    long long query(int idx) {
        long long res = 0;
        while (idx > 0) {
            res = (res + fenw[idx]) % MOD;
            idx -= (idx & -idx);
        }
        return res;
    }

    long long range_query(int l, int r) {
        if (l > r) return 0;
        long long left_part = (l-1>0) ? query(l-1) : 0;
        long long right_part = query(r);
        return (right_part - left_part + MOD) % MOD;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i=0; i<n; i++) {
        cin >> a[i];
    }

    vector<long long> pow2(MAXN, 1);
    for (int i=1; i<MAXN; i++) {
        pow2[i] = (pow2[i-1] * 2) % MOD;
    }

    long long inv2 = (MOD+1)/2;
    vector<long long> inv_pow2(MAXN, 1);
    for (int i=1; i<MAXN; i++) {
        inv_pow2[i] = (inv_pow2[i-1] * inv2) % MOD;
    }

    vector<int> f(n, 1);
    FenwMax fenw_f(n);
    for (int i=0; i<n; i++) {
        if (a[i] > 1) {
            int q = fenw_f.query(a[i]-1);
            f[i] = q + 1;
        } else {
            f[i] = 1;
        }
        fenw_f.update(a[i], f[i]);
    }

    vector<int> g(n, 1);
    FenwMax fenw_g(n);
    for (int i=n-1; i>=0; i--) {
        int b_val = n+1 - a[i];
        if (b_val > 1) {
            int q = fenw_g.query(b_val-1);
            g[i] = q + 1;
        } else {
            g[i] = 1;
        }
        fenw_g.update(b_val, g[i]);
    }

    int L = 0;
    for (int i=0; i<n; i++) {
        if (f[i] > L) L = f[i];
    }

    vector<int> F;
    for (int i=0; i<n; i++) {
        if (f[i] + g[i] - 1 == L) {
            F.push_back(i);
        }
    }

    vector< vector<int> > levels(L+1);
    for (int i : F) {
        int k = f[i];
        if (k <= L) {
            levels[k].push_back(a[i]);
        }
    }

    long long dp0 = 1;
    vector<long long> dp(n+1, 0);

    for (int k=1; k<=L; k++) {
        vector<int> A = levels[k];
        if (A.empty()) {
            continue;
        }
        int total = A.size();
        vector<int> prefix_count_arr(n+1, 0);
        for (int v : A) {
            if (v >= 1 && v <= n) {
                prefix_count_arr[v] = 1;
            }
        }
        for (int i=1; i<=n; i++) {
            prefix_count_arr[i] += prefix_count_arr[i-1];
        }

        vector<long long> F_arr(n+1, 0);
        for (int s=1; s<=n; s++) {
            F_arr[s] = dp[s] * pow2[prefix_count_arr[s]] % MOD;
        }

        FenwSum fenw_tree(n);
        for (int s=1; s<=n; s++) {
            fenw_tree.update(s, F_arr[s]);
        }

        vector<long long> next_dp_nonempty(n+1, 0);
        for (int v : A) {
            long long term0 = dp0;
            term0 = term0 * pow2[total - prefix_count_arr[v]] % MOD;

            long long sum_range = fenw_tree.range_query(v, n);
            long long term_positive = sum_range * inv_pow2[prefix_count_arr[v]] % MOD;

            next_dp_nonempty[v] = (term0 + term_positive) % MOD;
        }

        long long new_dp0 = dp0;
        vector<long long> new_dp = dp;
        for (int v : A) {
            new_dp[v] = (new_dp[v] + next_dp_nonempty[v]) % MOD;
        }

        dp0 = new_dp0;
        dp = new_dp;
    }

    long long valid_subsets = dp0;
    for (int i=1; i<=n; i++) {
        valid_subsets = (valid_subsets + dp[i]) % MOD;
    }

    long long non_critical_count = n - F.size();
    long long total_subsets = pow2[n];
    long long ans = (total_subsets - pow2[non_critical_count] * valid_subsets % MOD + MOD) % MOD;
    cout << ans << endl;

    return 0;
}