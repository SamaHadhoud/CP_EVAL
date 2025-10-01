Here is an O(N log N) C++ solution that follows the outlined approach (checking only necessary thresholds x, using sorting, binary searches, and prefix sums), with 64-bit arithmetic for costs.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    long long T;
    if (!(cin >> N >> K >> T)) return 0;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];

    if (K == N) {
        cout << 0 << '\n';
        return 0;
    }
    if (T == 0) {
        cout << -1 << '\n';
        return 0;
    }

    vector<int> B = A;
    sort(B.begin(), B.end());
    vector<long long> pref(N + 1, 0);
    for (int i = 0; i < N; ++i) pref[i + 1] = pref[i] + (long long)B[i];

    vector<int> events;
    // x must be in [0, T-1]
    events.push_back(0);
    events.push_back((int)(T - 1));
    for (int a : A) {
        if (0 <= a && a <= T - 1) events.push_back(a);
        if (a - 1 >= 0 && a - 1 <= T - 1) events.push_back(a - 1);
    }
    sort(events.begin(), events.end());
    events.erase(unique(events.begin(), events.end()), events.end());

    long long ans = LLONG_MAX;

    for (int x : events) {
        if (x < 0 || x > T - 1) continue; // safety, though events already filtered
        int l = upper_bound(B.begin(), B.end(), x) - B.begin(); // count Ai <= x
        if (K <= l) {
            // Push l-K largest in L to the right (to x+1)
            long long cost = (long long)(l - K) * ((long long)x + 1) + (pref[K] - pref[l]);
            if (cost < ans) ans = cost;
        } else {
            int rem = K - l; // need to bring this many from the right to the left
            int pos1 = lower_bound(B.begin(), B.end(), x + 1) - B.begin(); // first >= x+1
            int pos2 = lower_bound(B.begin(), B.end(), x + 2) - B.begin(); // first >= x+2
            int m = pos2 - pos1; // count of Ai == x+1
            int takeM = min(m, rem);
            int r = rem - takeM; // need this many from H (Ai >= x+2)
            int idxH = pos2;
            long long sumH = pref[idxH + r] - pref[idxH]; // sum of r smallest in H
            long long cost = (long long)takeM + sumH - (long long)r * x;
            if (cost < ans) ans = cost;
        }
    }

    cout << ans << '\n';
    return 0;
}