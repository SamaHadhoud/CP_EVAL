#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;
typedef long long ll;

const ll INF = 1e18;

struct Job {
    int x;
    ll l;
    int p;
    int r_i;
    Job(int x, ll l, int p, int r_i) : x(x), l(l), p(p), r_i(r_i) {}
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, n;
    cin >> T >> n;

    vector<Job> jobs;
    for (int i = 0; i < n; i++) {
        int x;
        ll l;
        int p;
        cin >> x >> l >> p;
        int r_i = (x + l) % T;
        jobs.emplace_back(x, l, p, r_i);
    }

    int M = 5 * T;
    vector<vector<ll>> F_prev(6, vector<ll>(T, INF));
    F_prev[0][0] = 0;

    vector<ll> res0(M + 1, INF);
    res0[0] = 0;

    for (int k = 1; k <= M; k++) {
        vector<ll> G(T, INF);
        for (const Job& job : jobs) {
            if (k < job.p) continue;
            int prev_index = (k - job.p) % 6;
            if (F_prev[prev_index][job.r_i] == INF) continue;
            ll candidate = job.l + F_prev[prev_index][job.r_i];
            if (candidate < G[job.x]) {
                G[job.x] = candidate;
            }
        }

        vector<ll> A(T, INF);
        vector<ll> B_vec(T, INF);
        for (int x = 0; x < T; x++) {
            if (G[x] < INF) {
                A[x] = G[x] + x;
                B_vec[x] = G[x] + x + T;
            }
        }

        vector<ll> suff_min(T, INF);
        if (T > 0) {
            suff_min[T - 1] = A[T - 1];
            for (int i = T - 2; i >= 0; i--) {
                suff_min[i] = min(A[i], suff_min[i + 1]);
            }
        }

        vector<ll> pref_min_B(T, INF);
        if (T > 0) {
            pref_min_B[0] = INF;
            for (int i = 1; i < T; i++) {
                pref_min_B[i] = min(pref_min_B[i - 1], B_vec[i - 1]);
            }
        }

        vector<ll> F_current(T, INF);
        for (int r = 0; r < T; r++) {
            ll option1 = INF;
            if (suff_min[r] != INF) {
                option1 = suff_min[r] - r;
            }
            ll option2 = INF;
            if (r > 0 && pref_min_B[r] != INF) {
                option2 = pref_min_B[r] - r;
            }
            F_current[r] = min(option1, option2);
        }

        F_prev[k % 6] = F_current;
        res0[k] = F_current[0];
    }

    ll best_cycle_profit = 0;
    ll best_cycle_time = INF;
    for (int k1 = 0; k1 <= M; k1++) {
        if (res0[k1] == INF) continue;
        for (int k2 = k1 + 1; k2 <= min(M, k1 + 50); k2++) {
            if (res0[k2] == INF) continue;
            ll time_diff = res0[k2] - res0[k1];
            if (time_diff <= 0) continue;
            ll profit_diff = k2 - k1;
            if (best_cycle_profit * time_diff < profit_diff * best_cycle_time) {
                best_cycle_profit = profit_diff;
                best_cycle_time = time_diff;
            } else if (best_cycle_profit * time_diff == profit_diff * best_cycle_time) {
                if (time_diff < best_cycle_time) {
                    best_cycle_time = time_diff;
                    best_cycle_profit = profit_diff;
                }
            }
        }
    }

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        ll z;
        cin >> z;
        ll best_profit = 0;
        for (int k = 0; k <= M; k++) {
            if (res0[k] <= z) {
                ll time_left = z - res0[k];
                ll add_profit = 0;
                if (best_cycle_time != INF) {
                    add_profit = (time_left / best_cycle_time) * best_cycle_profit;
                }
                if (k + add_profit > best_profit) {
                    best_profit = k + add_profit;
                }
            }
        }
        cout << best_profit << '\n';
    }

    return 0;
}