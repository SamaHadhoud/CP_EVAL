#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

const int MOD = 1000000007;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int R, C;
    cin >> R >> C;

    vector<string> grid(R);
    for (int i = 0; i < R; i++) {
        cin >> grid[i];
    }

    int nT = 1 << R;
    vector<int> base(C, 0);
    for (int j = 0; j < C; j++) {
        int mask = 0;
        for (int i = 0; i < R; i++) {
            if (grid[i][j] == '.') {
                mask |= (1 << i);
            }
        }
        base[j] = mask;
    }

    vector<long long> power2(R+1, 1);
    for (int i = 1; i <= R; i++) {
        power2[i] = (power2[i-1] * 2) % MOD;
    }

    vector<int> popcount_arr(nT, 0);
    for (int i = 0; i < nT; i++) {
        popcount_arr[i] = __builtin_popcount(i);
    }

    vector<long long> F(nT, 0);
    for (int T = 0; T < nT; T++) {
        vector<long long> non_empty_ways(C, 0);
        for (int j = 0; j < C; j++) {
            int avail = base[j] & ~T;
            avail &= (nT - 1);
            int a = popcount_arr[avail];
            if (a == 0) {
                non_empty_ways[j] = 0;
            } else {
                non_empty_ways[j] = (power2[a] - 1) % MOD;
            }
        }

        if (C == 0) {
            F[T] = 0;
        } else {
            long long dp0 = 1;
            long long dp1 = non_empty_ways[0];
            for (int j = 1; j < C; j++) {
                long long new_dp0 = dp1;
                long long new_dp1 = (dp0 + dp1) % MOD;
                new_dp1 = (new_dp1 * non_empty_ways[j]) % MOD;
                dp0 = new_dp0;
                dp1 = new_dp1;
            }
            F[T] = (dp0 + dp1) % MOD;
        }
    }

    long long ans = 0;
    for (int T = 0; T < nT; T++) {
        int cnt = popcount_arr[T];
        if (cnt % 2 == 0) {
            ans = (ans + F[T]) % MOD;
        } else {
            ans = (ans - F[T]) % MOD;
        }
    }

    ans %= MOD;
    if (ans < 0) ans += MOD;
    cout << ans << endl;

    return 0;
}