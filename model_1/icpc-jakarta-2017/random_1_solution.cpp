#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

const int MAX_M = 3000;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<double>> F(MAX_M+1, vector<double>(MAX_M+1, 0.0));
    for (int s = 0; s <= MAX_M; s++) {
        for (int a = 0; a <= s; a++) {
            int b = s - a;
            if (s == 0) {
                F[a][b] = 0.0;
            } else if (a == 0) {
                F[0][b] = (1.0 + b * F[0][b-1]) / b;
            } else if (b == 0) {
                F[a][0] = (1.0 + a * F[a-1][1]) / a;
            } else {
                F[a][b] = (1.0 + a * F[a-1][b+1] + b * F[a][b-1]) / (a + b);
            }
        }
    }

    int T;
    cin >> T;
    while (T--) {
        int N, K;
        cin >> N >> K;
        vector<int> A(K);
        for (int i = 0; i < K; i++) {
            cin >> A[i];
        }

        vector<int> freq(N+1, 0);
        int a = N, b = 0;
        for (int x : A) {
            if (x >= 1 && x <= N) {
                freq[x]++;
                if (freq[x] == 1) {
                    a--;
                    b++;
                } else if (freq[x] == 2) {
                    b--;
                }
            }
        }

        double ans = F[a][b] * N;
        cout << fixed << setprecision(9) << ans << '\n';
    }

    return 0;
}