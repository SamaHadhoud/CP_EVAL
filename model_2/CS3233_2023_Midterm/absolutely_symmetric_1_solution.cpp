#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    vector<vector<long long>> A(n, vector<long long>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> A[i][j];

    bool parity_ok = true;
    bool abs_sym = true;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if ((A[i][j] & 1LL) != (A[j][i] & 1LL)) parity_ok = false;
            long long u = A[i][j], v = A[j][i];
            long long au = u < 0 ? -u : u;
            long long av = v < 0 ? -v : v;
            if (au != av) abs_sym = false;
        }
    }

    if (!parity_ok) {
        cout << -1 << '\n';
        return 0;
    }

    if (abs_sym) {
        cout << 1 << '\n';
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (j) cout << ' ';
                cout << A[i][j];
            }
            cout << '\n';
        }
        return 0;
    }

    vector<vector<long long>> S(n, vector<long long>(n)), K(n, vector<long long>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            S[i][j] = (A[i][j] + A[j][i]) / 2;
            K[i][j] = (A[i][j] - A[j][i]) / 2;
        }
    }

    cout << 2 << '\n';
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j) cout << ' ';
            cout << S[i][j];
        }
        cout << '\n';
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j) cout << ' ';
            cout << K[i][j];
        }
        cout << '\n';
    }

    return 0;
}