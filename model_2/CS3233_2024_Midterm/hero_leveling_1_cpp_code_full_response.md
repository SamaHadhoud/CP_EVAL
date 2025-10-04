```cpp
#include <bits/stdc++.h>
using namespace std;

struct Quest {
    long long M, X;
    int T;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, K;
    long long L;
    if (!(cin >> N >> K >> L)) return 0;
    vector<Quest> qs(N);
    int maxT = 0;
    for (int i = 0; i < N; ++i) {
        cin >> qs[i].M >> qs[i].X >> qs[i].T;
        maxT = max(maxT, qs[i].T);
    }
    // Sort quests by requirement Mi
    sort(qs.begin(), qs.end(), [](const Quest& a, const Quest& b){
        if (a.M != b.M) return a.M < b.M;
        return a.T < b.T;
    });

    // Candidate lambdas: 0 and all distinct Ti
    vector<int> lambdas;
    lambdas.push_back(0);
    {
        vector<int> seen(maxT + 1, 0);
        for (const auto& q : qs) seen[q.T] = 1;
        for (int t = 1; t <= maxT; ++t) if (seen[t]) lambdas.push_back(t);
    }
    sort(lambdas.begin(), lambdas.end());
    lambdas.erase(unique(lambdas.begin(), lambdas.end()), lambdas.end());

    const long long INF = (1LL<<62);
    long long ans = INF;

    // Precompute an upper bound for total cost
    const int MAXCOST = 150 * N; // since Ti in [1..150]
    
    for (int lam : lambdas) {
        long long base = 1LL * K * lam;
        if (base >= ans) break; // cannot improve further

        // Compute costs for this lambda
        vector<int> c(N);
        long long totalC = 0;
        for (int i = 0; i < N; ++i) {
            int ci = qs[i].T - lam;
            if (ci < 0) ci = 0;
            c[i] = ci;
            totalC += ci;
        }
        if (totalC == 0) {
            // All costs are zero; just check feasibility (can we reach L?)
            long long level = 0;
            for (int i = 0; i < N; ++i) {
                if (level >= qs[i].M) {
                    level = min(L, level + qs[i].X);
                    if (level >= L) break;
                }
            }
            if (level >= L) {
                ans = min(ans, base); // bestCost = 0
            }
            continue;
        }

        // Bound cost dimension using current best answer
        long long boundLL = min<long long>(totalC, MAXCOST);
        if (ans < INF) {
            long long cap = ans - base - 1; // bestCost must be <= cap to improve
            if (cap < 0) continue;
            boundLL = min(boundLL, cap);
        }
        int B = (int)boundLL;
        if (B < 0) continue;

        // DP[b] = max reachable level with exact cost b
        vector<long long> dp(B + 1, -1);
        dp[0] = 0;
        int bestCost = B + 1;
        int high = 0; // upper bound on highest cost index possibly reachable

        for (int i = 0; i < N; ++i) {
            int ci = c[i];
            if (ci > B) continue; // cannot take this quest within current bound

            int limit = min(B, bestCost - 1);
            int max_b = limit - ci;
            if (max_b < 0) {
                // Even from cost 0, taking this quest exceeds bound
                // But still update high bound for later (coarse upper bound)
                if (high + ci <= B) high += ci; else high = B;
                continue;
            }
            if (max_b > high) max_b = high;

            long long Mi = qs[i].M;
            long long Xi = qs[i].X;

            for (int b = max_b; b >= 0; --b) {
                long long v = dp[b];
                if (v >= Mi) {
                    long long nv = v + Xi;
                    if (nv > L) nv = L;
                    if (nv > dp[b + ci]) {
                        dp[b + ci] = nv;
                        if (nv >= L && b + ci < bestCost) {
                            bestCost = b + ci;
                        }
                    }
                }
            }
            if (high + ci <= B) high += ci; else high = B;
        }

        if (bestCost <= B) {
            ans = min(ans, base + bestCost);
        }
    }

    cout << ans << '\n';
    return 0;
}
```