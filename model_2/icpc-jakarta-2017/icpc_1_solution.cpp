#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    vector<int> T(M);
    for (int j = 0; j < M; ++j) cin >> T[j];

    int bestCount = -1;
    int bestIndex = 1;

    for (int i = 1; i <= N; ++i) {
        int count = 0;
        for (int j = 0; j < M; ++j) {
            int s; cin >> s;
            if (s == T[j]) ++count;
        }
        if (count > bestCount || (count == bestCount && i < bestIndex)) {
            bestCount = count;
            bestIndex = i;
        }
    }

    cout << bestIndex << '\n';
    return 0;
}