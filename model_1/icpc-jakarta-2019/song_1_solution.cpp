#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    long long n, L, R, K;
    cin >> n >> L >> R >> K;
    vector<long long> A(n);
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    vector<long long> low(n);
    vector<long long> high(n);

    low[n-1] = L;
    high[n-1] = R;

    for (int i = n-2; i >= 0; i--) {
        if (A[i] < A[i+1]) {
            low[i] = max(L, low[i+1] - K);
            high[i] = min(R, high[i+1] - 1);
        } else if (A[i] == A[i+1]) {
            low[i] = max(L, low[i+1]);
            high[i] = min(R, high[i+1]);
        } else {
            low[i] = max(L, low[i+1] + 1);
            high[i] = min(R, high[i+1] + K);
        }

        if (low[i] > high[i]) {
            cout << -1 << endl;
            return 0;
        }
    }

    vector<long long> res(n);
    res[0] = low[0];

    for (int i = 0; i < n-1; i++) {
        if (A[i] < A[i+1]) {
            res[i+1] = max(low[i+1], res[i] + 1);
        } else if (A[i] == A[i+1]) {
            res[i+1] = res[i];
        } else {
            res[i+1] = max(low[i+1], res[i] - K);
        }
    }

    for (int i = 0; i < n; i++) {
        if (i > 0) 
            cout << " ";
        cout << res[i];
    }
    cout << endl;

    return 0;
}