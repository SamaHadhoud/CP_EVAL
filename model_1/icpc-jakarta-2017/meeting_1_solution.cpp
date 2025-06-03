#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <climits>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n, K, T;
    cin >> n >> K >> T;
    vector<long long> A(n);
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    if (T == 0) {
        if (K == n) {
            cout << 0 << endl;
        } else {
            cout << -1 << endl;
        }
        return 0;
    }

    if (K == n) {
        cout << 0 << endl;
        return 0;
    }

    sort(A.begin(), A.end());
    vector<long long> P(n + 1, 0);
    for (int i = 0; i < n; i++) {
        P[i + 1] = P[i] + A[i];
    }

    set<long long> cand;
    cand.insert(0);
    if (T - 1 >= 0) {
        cand.insert(T - 1);
    }
    for (long long a : A) {
        if (a >= 0 && a <= T - 1) {
            cand.insert(a);
        }
        if (a - 1 >= 0 && a - 1 <= T - 1) {
            cand.insert(a - 1);
        }
    }

    long long best = (long long)1e18;
    for (long long m : cand) {
        auto it = upper_bound(A.begin(), A.end(), m);
        long long r = it - A.begin();

        if (K <= r) {
            long long cost = (m + 1) * (r - K) + (P[K] - P[r]);
            if (cost < best) {
                best = cost;
            }
        } else {
            long long s = K - r;
            if (r + s > n) {
                continue;
            }
            long long cost = (P[r + s] - P[r]) - s * m;
            if (cost < best) {
                best = cost;
            }
        }
    }

    cout << best << endl;

    return 0;
}