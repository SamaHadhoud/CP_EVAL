#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C;
    if (!(cin >> C)) return 0;
    while (C--) {
        int n;
        cin >> n;
        vector<unsigned long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];

        unsigned long long A = 0;
        for (auto v : a) A |= v;  // maximum possible XOR is the OR

        vector<unsigned long long> x(n, 0);
        for (int b = 0; b <= 60; ++b) {
            unsigned long long mask = 1ULL << b;
            if (A & mask) {
                for (int i = 0; i < n; ++i) {
                    if (a[i] & mask) {
                        x[i] |= mask;  // assign this bit to exactly one xi
                        break;
                    }
                }
            }
        }

        cout << A << '\n';
        for (int i = 0; i < n; ++i) {
            if (i) cout << ' ';
            cout << x[i];
        }
        cout << '\n';
    }
    return 0;
}