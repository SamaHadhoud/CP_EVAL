#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, Q;
    if (!(cin >> N >> Q)) return 0;

    vector<int> pR(N + 1), pC(N + 1);
    for (int i = 1; i <= N; ++i) {
        long long x; cin >> x;
        pR[i] = x & 1;
    }
    for (int j = 1; j <= N; ++j) {
        long long x; cin >> x;
        pC[j] = x & 1;
    }

    // Build row blocks: consecutive rows with the same parity
    vector<int> rowBlock(N + 1), colBlock(N + 1);
    int b = 1;
    rowBlock[1] = b;
    for (int i = 2; i <= N; ++i) {
        if (pR[i] == pR[i - 1]) rowBlock[i] = b;
        else rowBlock[i] = ++b;
    }

    // Build column blocks: consecutive columns with the same parity
    b = 1;
    colBlock[1] = b;
    for (int j = 2; j <= N; ++j) {
        if (pC[j] == pC[j - 1]) colBlock[j] = b;
        else colBlock[j] = ++b;
    }

    // Answer queries: path exists iff both row blocks and column blocks match
    while (Q--) {
        int ra, ca, rb, cb;
        cin >> ra >> ca >> rb >> cb;
        if (rowBlock[ra] == rowBlock[rb] && colBlock[ca] == colBlock[cb]) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
    return 0;
}