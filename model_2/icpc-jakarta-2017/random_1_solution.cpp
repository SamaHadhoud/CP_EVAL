#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int MAXN = 3000;

    // Precompute F in triangular DP: F[s][y] where s = x + y, 0 <= y <= s
    vector<vector<double>> F(MAXN + 1);
    for (int s = 0; s <= MAXN; ++s) F[s].resize(s + 1, 0.0);
    // Base: F[0][0] = 0 is already set

    for (int s = 1; s <= MAXN; ++s) {
        double invs = 1.0 / s;
        for (int y = s; y >= 0; --y) {
            int x = s - y;
            double val = invs;
            if (x > 0) val += (x * invs) * F[s][y + 1];      // F[x-1][y+1] = F[s][y+1]
            if (y > 0) val += (y * invs) * F[s - 1][y - 1];  // F[x][y-1] = F[s-1][y-1]
            F[s][y] = val;
        }
    }

    int T;
    if (!(cin >> T)) return 0;

    vector<unsigned char> cnt(MAXN + 1, 0);
    vector<int> touched;
    touched.reserve(4096);

    cout.setf(std::ios::fixed);
    cout << setprecision(9);

    while (T--) {
        int N, K;
        cin >> N >> K;

        touched.clear();
        for (int i = 0; i < K; ++i) {
            int a; cin >> a;
            if (cnt[a] == 0) { cnt[a] = 1; touched.push_back(a); }
            else if (cnt[a] == 1) { cnt[a] = 2; }
            // if cnt[a] == 2: ignore
        }

        int U = 0; // >=1 occurrences
        int y = 0; // exactly 1 occurrence
        for (int a : touched) {
            if (cnt[a] >= 1) ++U;
            if (cnt[a] == 1) ++y;
            cnt[a] = 0; // reset for next test
        }

        int x = N - U;
        int s = x + y; // s > 0 as guaranteed, but handle s=0 too
        double ans = (s == 0 ? 0.0 : N * F[s][y]);
        cout << ans << '\n';
    }

    return 0;
}