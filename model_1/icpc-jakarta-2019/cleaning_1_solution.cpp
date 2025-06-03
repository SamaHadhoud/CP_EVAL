#include <iostream>
#include <vector>
using namespace std;
const int MOD = 1000000007;
const int MAXN = 100000;

vector<int> graph[MAXN + 10];
long long dp[MAXN + 10][3];

void dfs(int u, int parent) {
    dp[u][0] = 1;
    dp[u][1] = 0;
    dp[u][2] = 0;
    for (int v : graph[u]) {
        if (v == parent) continue;
        dfs(v, u);
        long long ways0 = dp[v][0];
        long long ways1 = dp[v][1];
        long long ways2 = dp[v][2];

        long long ndp0 = (dp[u][0] * (2 * ways0 + 2 * ways1 + ways2) % MOD
                      + (dp[u][1] * (ways0 + ways1) % MOD)
                      + (dp[u][2] * (ways0 + ways1) % MOD;
        ndp0 %= MOD;

        long long ndp1 = (dp[u][0] * (ways0 + ways1) % MOD)
                      + (dp[u][1] * ways0 % MOD);
        ndp1 %= MOD;

        long long ndp2 = (dp[u][0] * (ways0 + ways1) % MOD)
                      + (dp[u][2] * ways0 % MOD);
        ndp2 %= MOD;

        dp[u][0] = ndp0;
        dp[u][1] = ndp1;
        dp[u][2] = ndp2;
    }
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        u--; v--;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    dfs(0, -1);
    long long ans = (dp[0][0] + dp[0][1]) % MOD;
    printf("%lld\n", ans);
    return 0;
}