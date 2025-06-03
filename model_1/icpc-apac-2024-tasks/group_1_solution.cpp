#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <climits>
using namespace std;

const int MAXN = 1000000;
vector<int> divisors_list[MAXN + 1];

void precompute_divisors() {
    for (int i = 1; i <= MAXN; i++) {
        for (int j = i; j <= MAXN; j += i) {
            divisors_list[j].push_back(i);
        }
    }
}

long long gcd(long long a, long long b) {
    if (b == 0) 
        return a;
    return gcd(b, a % b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    precompute_divisors();

    int t;
    cin >> t;
    while (t--) {
        long long n, a1;
        cin >> n >> a1;
        vector<long long> arr(n - 1);
        for (int i = 0; i < n - 1; i++) {
            cin >> arr[i];
        }

        vector<int> divs = divisors_list[n];
        long long best_max = -1, best_min = -1;

        for (int k : divs) {
            if (k == 1) continue;

            if (k == n) {
                long long min_val = a1;
                long long max_val = a1;
                for (long long x : arr) {
                    if (x < min_val) min_val = x;
                    if (x > max_val) max_val = x;
                }
                if (best_max == -1) {
                    best_max = max_val;
                    best_min = min_val;
                } else {
                    if (max_val * best_min < best_max * min_val) {
                        best_max = max_val;
                        best_min = min_val;
                    }
                }
                continue;
            }

            vector<long long> cur(k, 0);
            cur[0] = a1;
            for (int i = 0; i < n - 1; i++) {
                int r = (i + 1) % k;
                cur[r] += arr[i];
            }

            multiset<long long> mset;
            for (int i = 0; i < k; i++) {
                mset.insert(cur[i]);
            }

            long long current_min = *mset.begin();
            long long current_max = *mset.rbegin();
            long long best_max_k = current_max;
            long long best_min_k = current_min;

            for (int pos = 1; pos < n; pos++) {
                int r1 = (pos - 1) % k;
                int r2 = pos % k;

                auto it1 = mset.find(cur[r1]);
                mset.erase(it1);
                auto it2 = mset.find(cur[r2]);
                mset.erase(it2);

                long long old_r1 = cur[r1];
                long long old_r2 = cur[r2];
                cur[r1] = old_r1 - a1 + arr[pos - 1];
                cur[r2] = old_r2 - arr[pos - 1] + a1;

                mset.insert(cur[r1]);
                mset.insert(cur[r2]);

                current_min = *mset.begin();
                current_max = *mset.rbegin();

                if (current_max * best_min_k < best_max_k * current_min) {
                    best_max_k = current_max;
                    best_min_k = current_min;
                }
            }

            if (best_max == -1) {
                best_max = best_max_k;
                best_min = best_min_k;
            } else {
                if (best_max_k * best_min < best_max * best_min_k) {
                    best_max = best_max_k;
                    best_min = best_min_k;
                }
            }
        }

        long long g = gcd(best_max, best_min);
        best_max /= g;
        best_min /= g;
        cout << best_max << " " << best_min << "\n";
    }
    return 0;
}