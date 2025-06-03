#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<int> A(n+1);
    for (int i = 1; i <= n; i++) {
        cin >> A[i];
    }

    vector<int> P(n+1, 0);
    vector<int> break_cond(n+1, 0);
    int total = 0;

    P[0] = 0;
    for (int i = 1; i <= n; i++) {
        P[i] = max(P[i-1], A[i]);
        if (P[i] == i) {
            break_cond[i] = 1;
            total++;
        } else {
            break_cond[i] = 0;
        }
    }

    vector<int> output;
    output.push_back(total);

    for (int idx = 0; idx < m; idx++) {
        int u, v;
        cin >> u >> v;

        swap(A[u], A[v]);

        int start = min(u, v);
        int current_max = (start > 1) ? P[start-1] : 0;
        bool encountered_n = false;

        for (int i = start; i <= n; i++) {
            current_max = max(current_max, A[i]);
            P[i] = current_max;
            
            if (!encountered_n && current_max == n && i < n) {
                encountered_n = true;
                int new_cond = (current_max == i) ? 1 : 0;
                if (new_cond != break_cond[i]) {
                    if (break_cond[i]) {
                        total--;
                    } else {
                        total++;
                    }
                    break_cond[i] = new_cond;
                }
                for (int j = i+1; j < n; j++) {
                    if (break_cond[j] == 1) {
                        total--;
                        break_cond[j] = 0;
                    }
                }
                if (break_cond[n] != 1) {
                    total++;
                    break_cond[n] = 1;
                }
                for (int j = i+1; j <= n; j++) {
                    P[j] = n;
                }
                break;
            } else {
                int new_cond = (current_max == i) ? 1 : 0;
                if (new_cond != break_cond[i]) {
                    if (break_cond[i]) {
                        total--;
                    } else {
                        total++;
                    }
                    break_cond[i] = new_cond;
                }
            }
        }

        if (!encountered_n) {
            if (break_cond[n] != 1) {
                total++;
                break_cond[n] = 1;
            }
        }

        output.push_back(total);
    }

    for (int val : output) {
        cout << val << '\n';
    }

    return 0;
}