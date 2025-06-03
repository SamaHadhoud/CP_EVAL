#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<long long>> A(n, vector<long long>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((A[i][j] & 1) != (A[j][i] & 1)) {
                cout << -1 << endl;
                return 0;
            }
        }
    }

    bool abs_sym = true;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long a_val = A[i][j];
            long long b_val = A[j][i];
            if (a_val < 0) a_val = -a_val;
            if (b_val < 0) b_val = -b_val;
            if (a_val != b_val) {
                abs_sym = false;
                break;
            }
        }
        if (!abs_sym) break;
    }

    if (abs_sym) {
        cout << 1 << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << A[i][j];
                if (j < n - 1) cout << " ";
            }
            cout << endl;
        }
    } else {
        cout << 2 << endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    cout << 0;
                } else {
                    long long diff = A[i][j] - A[j][i];
                    cout << diff / 2;
                }
                if (j < n - 1) cout << " ";
            }
            cout << endl;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    cout << A[i][j];
                } else {
                    long long sum_val = A[i][j] + A[j][i];
                    cout << sum_val / 2;
                }
                if (j < n - 1) cout << " ";
            }
            cout << endl;
        }
    }

    return 0;
}