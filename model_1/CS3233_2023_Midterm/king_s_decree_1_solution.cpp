#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int C;
    cin >> C;
    vector<long long> results;
    for (int c = 0; c < C; c++) {
        int n;
        cin >> n;
        vector<long long> w(n);
        vector<long long> l(n);
        long long total_wealth = 0;
        for (int i = 0; i < n; i++) {
            cin >> w[i];
            total_wealth += w[i];
        }
        for (int i = 0; i < n; i++) {
            cin >> l[i];
        }
        long long total_l = accumulate(l.begin(), l.end(), 0LL);
        sort(l.begin(), l.end());
        vector<long long> distinct;
        vector<int> freqs;
        for (int i = 0; i < n; ) {
            int j = i;
            while (j < n && l[j] == l[i]) {
                j++;
            }
            distinct.push_back(l[i]);
            freqs.push_back(j - i);
            i = j;
        }
        long long cum_freq = 0;
        long long cum_sum = 0;
        long long best = 0;
        int num_d = distinct.size();
        for (int i = 0; i < num_d; i++) {
            long long d = distinct[i];
            int cnt = freqs[i];
            cum_freq += cnt;
            cum_sum += static_cast<long long>(cnt) * d;
            long long numerator = total_wealth - total_l + cum_sum;
            long long candidate_val = numerator / cum_freq;
            long long candidate_seg;
            if (i < num_d - 1) {
                candidate_seg = min(candidate_val, distinct[i+1] - 1);
            } else {
                candidate_seg = candidate_val;
            }
            if (candidate_seg >= d) {
                if (candidate_seg > best) {
                    best = candidate_seg;
                }
            }
        }
        results.push_back(best);
    }
    for (long long res : results) {
        cout << res << '\n';
    }
    return 0;
}