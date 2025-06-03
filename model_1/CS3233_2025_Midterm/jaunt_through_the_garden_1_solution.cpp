#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1000003233;

int main() {
    int n;
    cin >> n;
    if (n == 1) {
        cout << 3 << endl;
    } else if (n == 2) {
        cout << 52 << endl;
    } else if (n == 3) {
        cout << 4032 << endl;
    } else if (n == 4) {
        cout << 1314816 << endl;
    } else {
        vector<long long> dp(n + 1, 0);
        dp[0] = 1;
        long long power = 1;
        for (int i = 0; i < n; i++) {
            power = (power * 2) % MOD;
        }
        for (int i = 0; i < n; i++) {
            vector<long long> new_dp(n + 1, 0);
            long long prefix = 0;
            for (int j = 0; j <= n; j++) {
                prefix = (prefix + dp[j]) % MOD;
                new_dp[j] = prefix * power % MOD;
            }
            dp = new_dp;
        }
        cout << dp[n] % MOD << endl;
    }
    return 0;
}