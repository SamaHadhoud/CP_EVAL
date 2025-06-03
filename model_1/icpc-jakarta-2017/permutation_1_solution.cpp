#include <iostream>
#include <vector>
using namespace std;

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> P(n);
    for (int i = 0; i < n; i++) {
        cin >> P[i];
        P[i]--;
    }

    vector<bool> visited(n, false);
    vector<vector<int>> cycles;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            vector<int> cycle;
            int cur = i;
            while (!visited[cur]) {
                visited[cur] = true;
                cycle.push_back(cur);
                cur = P[cur];
            }
            cycles.push_back(cycle);
        }
    }

    vector<int> cycle_lengths;
    for (auto &cycle : cycles) {
        cycle_lengths.push_back(cycle.size());
    }
    int k = 1;
    for (int L : cycle_lengths) {
        int g = gcd(k, L);
        k = (k / g) * L;
    }

    vector<vector<int>> perms(k, vector<int>(n));
    for (int i = 0; i < k; i++) {
        int exponent = i + 1;
        for (auto &cycle : cycles) {
            int L = cycle.size();
            for (int j = 0; j < L; j++) {
                int new_index_in_cycle = (j + exponent) % L;
                perms[i][cycle[j]] = cycle[new_index_in_cycle];
            }
        }
    }

    vector<int> idx(k);
    for (int i = 0; i < k; i++) {
        idx[i] = i;
    }

    for (int d = n - 1; d >= 0; d--) {
        vector<int> count(n, 0);
        for (int i = 0; i < k; i++) {
            int x = perms[idx[i]][d];
            count[x]++;
        }

        vector<int> pos(n, 0);
        if (n > 0) {
            for (int x = 1; x < n; x++) {
                pos[x] = pos[x - 1] + count[x - 1];
            }
        }

        vector<int> new_idx(k);
        for (int i = 0; i < k; i++) {
            int perm_index = idx[i];
            int x = perms[perm_index][d];
            new_idx[pos[x]] = perm_index;
            pos[x]++;
        }

        idx = new_idx;
    }

    vector<int> result_exponent(k);
    for (int i = 0; i < k; i++) {
        result_exponent[i] = idx[i] + 1;
    }

    for (int i = 0; i < q; i++) {
        int K;
        cin >> K;
        cout << result_exponent[K - 1] << '\n';
    }

    return 0;
}