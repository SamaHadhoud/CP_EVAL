```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    long long L, R, K;
    if (!(cin >> N >> L >> R >> K)) return 0;
    vector<long long> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];

    vector<long long> lo(N), hi(N);
    lo[N-1] = L;
    hi[N-1] = R;

    for (int i = N - 2; i >= 0; --i) {
        if (A[i] < A[i+1]) {
            long long nlo = lo[i+1] - K;
            long long nhi = hi[i+1] - 1;
            lo[i] = max(L, nlo);
            hi[i] = min(R, nhi);
        } else if (A[i] == A[i+1]) {
            lo[i] = max(L, lo[i+1]);
            hi[i] = min(R, hi[i+1]);
        } else { // A[i] > A[i+1]
            long long nlo = lo[i+1] + 1;
            long long nhi = hi[i+1] + K;
            lo[i] = max(L, nlo);
            hi[i] = min(R, nhi);
        }
        if (lo[i] > hi[i]) {
            cout << -1 << '\n';
            return 0;
        }
    }

    vector<long long> B(N);
    B[0] = lo[0];
    for (int i = 0; i + 1 < N; ++i) {
        if (A[i] < A[i+1]) {
            long long Lcand = max(lo[i+1], B[i] + 1);
            long long Rcand = min(hi[i+1], B[i] + K);
            if (Lcand > Rcand) { cout << -1 << '\n'; return 0; }
            B[i+1] = Lcand;
        } else if (A[i] == A[i+1]) {
            if (B[i] < lo[i+1] || B[i] > hi[i+1]) { cout << -1 << '\n'; return 0; }
            B[i+1] = B[i];
        } else { // A[i] > A[i+1]
            long long Lcand = max(lo[i+1], B[i] - K);
            long long Rcand = min(hi[i+1], B[i] - 1);
            if (Lcand > Rcand) { cout << -1 << '\n'; return 0; }
            B[i+1] = Lcand;
        }
    }

    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << B[i];
    }
    cout << '\n';
    return 0;
}
```