#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int MAXN = 40;
const int MAXM = 16;
const int PEGS = 3;
const int64 INF = (int64)4e18;

int N, M;
int64 Rcost[PEGS][PEGS];
int64 Ttower[MAXN + 1][PEGS][PEGS]; // T[size][from][to]

inline int third(int a, int b) {
    return 3 - a - b; // valid because pegs are 0,1,2
}

// DistToTower(A, t, peg): minimal cost to move disks 1..t from arrangement A to a tower on 'peg'
int64 DistToTower(const vector<int>& A, int t, int peg) {
    if (t <= 0) return 0;
    int k = t;
    while (k >= 1 && A[k] == peg) --k;
    if (k == 0) return 0;
    int a = A[k];
    int c = third(a, peg);
    return DistToTower(A, k - 1, c) + Rcost[a][peg] + Ttower[k - 1][c][peg];
}

// DistFromTower(t, peg, B): minimal cost to move disks 1..t from a tower on 'peg' to arrangement B
int64 DistFromTower(int t, int peg, const vector<int>& B) {
    if (t <= 0) return 0;
    int k = t;
    while (k >= 1 && B[k] == peg) --k;
    if (k == 0) return 0;
    int b = B[k];
    int c = third(peg, b);
    return Ttower[k - 1][peg][c] + Rcost[peg][b] + DistFromTower(k - 1, c, B);
}

// Dist(A, B, t): minimal cost to move disks 1..t from arrangement A to arrangement B
int64 Dist(const vector<int>& A, const vector<int>& B, int t) {
    if (t <= 0) return 0;
    int k = t;
    while (k >= 1 && A[k] == B[k]) --k;
    if (k == 0) return 0;
    int a = A[k], b = B[k];
    int c = third(a, b);
    return DistToTower(A, k - 1, c) + Rcost[a][b] + DistFromTower(k - 1, c, B);
}

bool sameConfig(const vector<int>& A, const vector<int>& B) {
    for (int i = 1; i <= N; ++i) if (A[i] != B[i]) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> M)) return 0;
    for (int i = 0; i < PEGS; ++i)
        for (int j = 0; j < PEGS; ++j)
            cin >> Rcost[i][j];

    vector<vector<int>> pos(M + 1, vector<int>(N + 1, -1)); // pos[node][disk] = peg

    // Read initial configuration (node 0)
    for (int p = 0; p < PEGS; ++p) {
        int K; cin >> K;
        for (int i = 0; i < K; ++i) {
            int d; cin >> d;
            pos[0][d] = p;
        }
    }

    // Read M target configurations (nodes 1..M)
    for (int g = 1; g <= M; ++g) {
        for (int p = 0; p < PEGS; ++p) {
            int K; cin >> K;
            for (int i = 0; i < K; ++i) {
                int d; cin >> d;
                pos[g][d] = p;
            }
        }
    }

    // Precompute Ttower[size][from][to]
    for (int a = 0; a < PEGS; ++a)
        for (int b = 0; b < PEGS; ++b)
            Ttower[0][a][b] = 0;
    for (int s = 1; s <= N; ++s) {
        for (int a = 0; a < PEGS; ++a) {
            for (int b = 0; b < PEGS; ++b) {
                if (a == b) {
                    Ttower[s][a][b] = 0;
                } else {
                    int c = third(a, b);
                    Ttower[s][a][b] = Ttower[s - 1][a][c] + Rcost[a][b] + Ttower[s - 1][c][b];
                }
            }
        }
    }

    // Pairwise directed distances between nodes (initial + targets)
    int nodes = M + 1;
    vector<vector<int64>> D(nodes, vector<int64>(nodes, 0));
    for (int u = 0; u < nodes; ++u) {
        for (int v = 0; v < nodes; ++v) {
            D[u][v] = Dist(pos[u], pos[v], N);
        }
    }

    // Cost to end with all disks on a single peg from each node
    vector<int64> endCost(nodes, INF);
    for (int u = 0; u < nodes; ++u) {
        int64 best = INF;
        for (int p = 0; p < PEGS; ++p) {
            best = min(best, DistToTower(pos[u], N, p));
        }
        endCost[u] = best;
    }

    // Subset DP over target visits
    int fullMask = (M == 0) ? 0 : ((1 << M) - 1);
    vector<vector<int64>> dp(1 << M, vector<int64>(nodes, INF));

    int initialMask = 0;
    for (int i = 1; i <= M; ++i) {
        if (sameConfig(pos[0], pos[i])) {
            initialMask |= (1 << (i - 1));
        }
    }
    dp[initialMask][0] = 0;

    for (int mask = 0; mask <= fullMask; ++mask) {
        for (int u = 0; u < nodes; ++u) {
            int64 cur = dp[mask][u];
            if (cur >= INF) continue;
            for (int v = 1; v <= M; ++v) {
                int bit = v - 1;
                if (!(mask & (1 << bit))) {
                    int nmask = mask | (1 << bit);
                    int64 cand = cur + D[u][v];
                    if (cand < dp[nmask][v]) dp[nmask][v] = cand;
                }
            }
        }
    }

    int64 ans = INF;
    for (int u = 0; u < nodes; ++u) {
        int64 base = dp[fullMask][u];
        if (base >= INF) continue;
        ans = min(ans, base + endCost[u]);
    }

    cout << ans << "\n";
    return 0;
}