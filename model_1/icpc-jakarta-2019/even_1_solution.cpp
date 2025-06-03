#include <cstdio>
#include <vector>
using namespace std;

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    vector<int> R(n);
    vector<int> C(n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &R[i]);
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &C[i]);
    }

    vector<int> r_parity(n);
    vector<int> c_parity(n);
    for (int i = 0; i < n; i++) {
        r_parity[i] = R[i] % 2;
        c_parity[i] = C[i] % 2;
    }

    vector<int> row0_group(n, -1);
    vector<int> row1_group(n, -1);
    vector<int> col0_group(n, -1);
    vector<int> col1_group(n, -1);

    int current_start = -1;
    for (int i = 0; i < n; i++) {
        if (r_parity[i] == 0) {
            if (i == 0 || r_parity[i-1] != 0) {
                current_start = i;
            }
            row0_group[i] = current_start;
        } else {
            if (i == 0 || r_parity[i-1] != 1) {
                current_start = i;
            }
            row1_group[i] = current_start;
        }
    }

    current_start = -1;
    for (int j = 0; j < n; j++) {
        if (c_parity[j] == 0) {
            if (j == 0 || c_parity[j-1] != 0) {
                current_start = j;
            }
            col0_group[j] = current_start;
        } else {
            if (j == 0 || c_parity[j-1] != 1) {
                current_start = j;
            }
            col1_group[j] = current_start;
        }
    }

    while (q--) {
        int ra, ca, rb, cb;
        scanf("%d %d %d %d", &ra, &ca, &rb, &cb);
        ra--; ca--; rb--; cb--;

        if (r_parity[ra] != r_parity[rb]) {
            printf("NO\n");
        } else {
            if (r_parity[ra] == 0) {
                if (row0_group[ra] == row0_group[rb] && col0_group[ca] == col0_group[cb]) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
            } else {
                if (row1_group[ra] == row1_group[rb] && col1_group[ca] == col1_group[cb]) {
                    printf("YES\n");
                } else {
                    printf("NO\n");
                }
            }
        }
    }

    return 0;
}