#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

typedef unsigned long long ull;

int main() {
    ull K;
    string S;
    cin >> K;
    cin >> S;
    int n = S.size();

    ull BIG = K + 1;

    for (int L = n; L >= 1; L--) {
        if (L == 1) {
            cout << n - 1 << endl;
            return 0;
        }

        vector<ull> dp_prev(n, BIG);
        for (int i = 0; i < n; i++) {
            if (S[i] == '0') {
                dp_prev[i] = 0;
            } else {
                dp_prev[i] = 1;
            }
        }

        for (int j = 2; j <= L; j++) {
            vector<ull> dp_curr(n, BIG);
            ull min_so_far = BIG;

            for (int i = 0; i < n; i++) {
                if (min_so_far != BIG) {
                    ull bit_val = (S[i] == '1') ? 1 : 0;
                    ull candidate = min_so_far * 2 + bit_val;
                    if (candidate <= K) {
                        dp_curr[i] = candidate;
                    }
                }

                if (dp_prev[i] != BIG && dp_prev[i] <= K) {
                    if (dp_prev[i] != 0) {
                        if (min_so_far == BIG) {
                            min_so_far = dp_prev[i];
                        } else {
                            if (dp_prev[i] < min_so_far) {
                                min_so_far = dp_prev[i];
                            }
                        }
                    }
                }
            }
            dp_prev = dp_curr;
        }

        for (ull val : dp_prev) {
            if (val <= K) {
                cout << n - L << endl;
                return 0;
            }
        }
    }

    cout << n - 1 << endl;
    return 0;
}