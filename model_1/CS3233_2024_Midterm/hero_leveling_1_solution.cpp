#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

typedef long long ll;

const int MAX_SUM = 23000;
const ll INF = 1.5e18;

int main() {
    int n, k;
    ll L;
    cin >> n >> k >> L;
    vector<tuple<ll, ll, int>> quests(n);
    for (int i = 0; i < n; i++) {
        ll M, X;
        int T;
        cin >> M >> X >> T;
        quests[i] = make_tuple(M, X, T);
    }
    sort(quests.begin(), quests.end());

    vector<vector<ll>> dp(n + 1, vector<ll>(MAX_SUM + 1, -INF));
    dp[0][0] = 0;

    for (int idx = 0; idx < n; idx++) {
        auto [M, X, T] = quests[idx];
        auto new_dp = dp;
        for (int j = n - 1; j >= 0; j--) {
            for (int s = 0; s <= MAX_SUM; s++) {
                if (dp[j][s] == -INF) continue;
                if (dp[j][s] < M) continue;
                ll new_level = dp[j][s] + X;
                if (new_level > INF) new_level = INF;
                int next_j = j + 1;
                int next_s = s;
                if (next_j > k) {
                    if (s + T > MAX_SUM) continue;
                    next_s = s + T;
                }
                if (new_level > new_dp[next_j][next_s]) {
                    new_dp[next_j][next_s] = new_level;
                }
            }
        }
        dp = new_dp;
    }

    ll ans = INF;
    for (int j = 1; j <= n; j++) {
        for (int s = 0; s <= MAX_SUM; s++) {
            if (dp[j][s] >= L) {
                if (j <= k) {
                    int total_trauma = 0;
                    for (int i = 0; i < j; i++) {
                        auto [M, X, T] = quests[i];
                        total_trauma += T;
                    }
                    if (total_trauma < ans) {
                        ans = total_trauma;
                    }
                } else {
                    ll trauma_rating = total_trauma - s;
                    if (trauma_rating < ans) {
                        ans = trauma_rating;
                    }
                }
            }
        }
    }
    cout << ans << endl;
    return 0;
}