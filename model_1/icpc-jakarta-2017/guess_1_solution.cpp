#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    long long N, K;
    cin >> N >> K;
    string s;
    cin >> s;
    int M = s.size();

    vector<long long> dp(M + 1, 0);
    dp[M] = 1;
    for (int i = M - 1; i >= 0; i--) {
        if (s[i] == '=') {
            dp[i] = 1;
        } else {
            if (i == M - 1) {
                dp[i] = 2;
            } else {
                dp[i] = dp[i + 1] + 1;
            }
        }
    }

    long long L = 1, R = N;
    vector<long long> ans;

    for (int i = 0; i < M; i++) {
        if (R < L || R - L + 1 < dp[i]) {
            cout << -1 << endl;
            return 0;
        }

        if (s[i] == '=') {
            if (L != R) {
                cout << -1 << endl;
                return 0;
            }
            ans.push_back(L);
        } else if (s[i] == '<') {
            long long candidate1 = (L + R + 1) / 2;
            long long candidate2 = L + dp[i + 1];
            long long g = max(candidate1, candidate2);
            if (g < L || g > R) {
                cout << -1 << endl;
                return 0;
            }
            ans.push_back(g);
            R = g - 1;
        } else if (s[i] == '>') {
            long long candidate1 = (L + R - 1) / 2;
            long long candidate2 = R - dp[i + 1];
            long long g = min(candidate1, candidate2);
            if (g < L || g > R) {
                cout << -1 << endl;
                return 0;
            }
            ans.push_back(g);
            L = g + 1;
        }
    }

    for (int i = 0; i < ans.size(); i++) {
        if (i > 0) cout << " ";
        cout << ans[i];
    }
    cout << endl;

    return 0;
}