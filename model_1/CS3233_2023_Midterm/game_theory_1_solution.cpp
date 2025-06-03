#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const int S = 550;
const long long NEG_INF = -1e18;
const long long POS_INF = 1e18;

vector<vector<long long>> max1_val(S+1);
vector<vector<long long>> min0_val(S+1);
vector<vector<int>> timestamp_vec(S+1);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    for (int d = 1; d <= S; d++) {
        max1_val[d].resize(d, NEG_INF);
        min0_val[d].resize(d, POS_INF);
        timestamp_vec[d].resize(d, 0);
    }

    int test_case_id = 0;
    int C;
    cin >> C;
    while (C--) {
        test_case_id++;
        int n;
        cin >> n;
        vector<int> p(n);
        vector<long long> a(n);
        for (int i = 0; i < n; i++) cin >> p[i];
        for (int i = 0; i < n; i++) cin >> a[i];

        vector<long long> dp0(n+1, 0);
        vector<long long> dp1(n+1, 0);

        for (int i = n; i >= 1; i--) {
            int d_val = p[i-1];
            long long a_i = a[i-1];
            long long candidate0 = NEG_INF;
            long long candidate1 = POS_INF;

            if (d_val <= S) {
                int r = i % d_val;
                if (timestamp_vec[d_val][r] == test_case_id) {
                    candidate0 = max1_val[d_val][r];
                    candidate1 = min0_val[d_val][r];
                }
            } else {
                for (int j = i + d_val; j <= n; j += d_val) {
                    if (dp1[j] > candidate0) candidate0 = dp1[j];
                    if (dp0[j] < candidate1) candidate1 = dp0[j];
                }
            }

            if (candidate0 == NEG_INF) {
                dp0[i] = a_i;
            } else {
                dp0[i] = max(a_i, a_i + candidate0);
            }

            if (candidate1 == POS_INF) {
                dp1[i] = a_i;
            } else {
                dp1[i] = min(a_i, a_i + candidate1);
            }

            for (int d_prime = 1; d_prime <= S; d_prime++) {
                int r = i % d_prime;
                if (timestamp_vec[d_prime][r] != test_case_id) {
                    timestamp_vec[d_prime][r] = test_case_id;
                    max1_val[d_prime][r] = NEG_INF;
                    min0_val[d_prime][r] = POS_INF;
                }
                if (dp1[i] > max1_val[d_prime][r]) {
                    max1_val[d_prime][r] = dp1[i];
                }
                if (dp0[i] < min0_val[d_prime][r]) {
                    min0_val[d_prime][r] = dp0[i];
                }
            }
        }

        for (int i = 1; i <= n; i++) {
            cout << dp0[i];
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
    return 0;
}