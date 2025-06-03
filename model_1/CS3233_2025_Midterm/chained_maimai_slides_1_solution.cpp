#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>
using namespace std;

const int MAX_M = 1000;
const int MAX_N = 8;
const int BOUND = 8;
const int INF = 1e9;

int M, K;
vector<vector<int>> edges;

int dp[1<<MAX_N][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1];

void reset_dp() {
    for (int mask = 0; mask < (1<<MAX_N); mask++) {
        for (int a0 = 0; a0 <= 2*BOUND; a0++) for (int a1 = 0; a1 <= 2*BOUND; a1++) for (int a2 = 0; a2 <= 2*BOUND; a2++) for (int a3 = 0; a3 <= 2*BOUND; a3++) for (int a4 = 0; a4 <= 2*BOUND; a4++) for (int a5 = 0; a5 <= 2*BOUND; a5++) for (int a6 = 0; a6 <= 2*BOUND; a6++) {
            dp[mask][a0][a1][a2][a3][a4][a5][a6] = -1;
        }
    }
}

inline int get_index(int flow_val) {
    return flow_val + BOUND;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> M >> K;
    for (int i = 0; i < M; i++) {
        int x, y, v;
        cin >> x >> y >> v;
        edges.push_back({v, x, y});
    }
    sort(edges.begin(), edges.end());

    int ans = INF;
    int L = 0;
    reset_dp();

    dp[0][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)] = 0;

    for (int R = 0; R < M; R++) {
        int v = edges[R][0], u_node = edges[R][1]-1, v_node = edges[R][2]-1;

        int new_dp[1<<MAX_N][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1][2*BOUND+1];
        memcpy(new_dp, dp, sizeof(dp));

        for (int mask = 0; mask < (1<<MAX_N); mask++) {
            for (int a0 = 0; a0 <= 2*BOUND; a0++) for (int a1 = 0; a1 <= 2*BOUND; a1++) for (int a2 = 0; a2 <= 2*BOUND; a2++) for (int a3 = 0; a3 <= 2*BOUND; a3++) for (int a4 = 0; a4 <= 2*BOUND; a4++) for (int a5 = 0; a5 <= 2*BOUND; a5++) for (int a6 = 0; a6 <= 2*BOUND; a6++) {
                if (dp[mask][a0][a1][a2][a3][a4][a5][a6] == -1) continue;

                int new_mask = mask | (1 << u_node);
                int f[7] = {a0 - BOUND, a1 - BOUND, a2 - BOUND, a3 - BOUND, a4 - BOUND, a5 - BOUND, a6 - BOUND};

                if (u_node < 7) {
                    f[u_node]++;
                }
                if (v_node < 7) {
                    f[v_node]--;
                }

                bool valid = true;
                for (int i = 0; i < 7; i++) {
                    if (f[i] < -BOUND || f[i] > BOUND) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) continue;

                int n0 = get_index(f[0]), n1 = get_index(f[1]), n2 = get_index(f[2]), n3 = get_index(f[3]), n4 = get_index(f[4]), n5 = get_index(f[5]), n6 = get_index(f[6]);
                int new_count = dp[mask][a0][a1][a2][a3][a4][a5][a6] + 1;
                if (new_dp[new_mask][n0][n1][n2][n3][n4][n5][n6] < new_count) {
                    new_dp[new_mask][n0][n1][n2][n3][n4][n5][n6] = new_count;
                }
            }
        }
        memcpy(dp, new_dp, sizeof(dp));

        bool found = false;
        while (L <= R) {
            if (edges[R][0] - edges[L][0] >= ans) {
                for (int mask = 0; mask < (1<<MAX_N); mask++) {
                    for (int a0 = 0; a0 <= 2*BOUND; a0++) for (int a1 = 0; a1 <= 2*BOUND; a1++) for (int a2 = 0; a2 <= 2*BOUND; a2++) for (int a3 = 0; a3 <= 2*BOUND; a3++) for (int a4 = 0; a4 <= 2*BOUND; a4++) for (int a5 = 0; a5 <= 2*BOUND; a5++) for (int a6 = 0; a6 <= 2*BOUND; a6++) {
                        if (dp[mask][a0][a1][a2][a3][a4][a5][a6] >= K) {
                            int f0 = a0 - BOUND, f1 = a1 - BOUND, f2 = a2 - BOUND, f3 = a3 - BOUND, f4 = a4 - BOUND, f5 = a5 - BOUND, f6 = a6 - BOUND;
                            if (mask == (1<<MAX_N)-1 && f0==0 && f1==0 && f2==0 && f3==0 && f4==0 && f5==0 && f6==0) {
                                ans = min(ans, edges[R][0] - edges[L][0]);
                                found = true;
                            }
                        }
                    }
                }
                if (found) {
                    int u_node = edges[L][1]-1, v_node = edges[L][2]-1;
                    for (int mask = 0; mask < (1<<MAX_N); mask++) {
                        for (int a0 = 0; a0 <= 2*BOUND; a0++) for (int a1 = 0; a1 <= 2*BOUND; a1++) for (int a2 = 0; a2 <= 2*BOUND; a2++) for (int a3 = 0; a3 <= 2*BOUND; a3++) for (int a4 = 0; a4 <= 2*BOUND; a4++) for (int a5 = 0; a5 <= 2*BOUND; a5++) for (int a6 = 0; a6 <= 2*BOUND; a6++) {
                            if (dp[mask][a0][a1][a2][a3][a4][a5][a6] == -1) continue;
                            int new_mask = mask;
                            int f[7] = {a0 - BOUND, a1 - BOUND, a2 - BOUND, a3 - BOUND, a4 - BOUND, a5 - BOUND, a6 - BOUND};

                            if (u_node < 7) {
                                f[u_node]--;
                            }
                            if (v_node < 7) {
                                f[v_node]++;
                            }

                            bool valid = true;
                            for (int i = 0; i < 7; i++) {
                                if (f[i] < -BOUND || f[i] > BOUND) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (!valid) continue;

                            int n0 = get_index(f[0]), n1 = get_index(f[1]), n2 = get_index(f[2]), n3 = get_index(f[3]), n4 = get_index(f[4]), n5 = get_index(f[5]), n6 = get_index(f[6]);
                            int new_count = dp[mask][a0][a1][a2][a3][a4][a5][a6] - 1;
                            if (new_count > dp[new_mask][n0][n1][n2][n3][n4][n5][n6]) {
                                dp[new_mask][n0][n1][n2][n3][n4][n5][n6] = new_count;
                            }
                        }
                    }
                    L++;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }

    if (ans == INF) {
        bool found = false;
        for (int L_fix = 0; L_fix < M; L_fix++) {
            for (int R_fix = L_fix; R_fix < M; R_fix++) {
                int range_val = edges[R_fix][0] - edges[L_fix][0];
                if (range_val >= ans) continue;
                reset_dp();
                dp[0][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)] = 0;
                for (int i = L_fix; i <= R_fix; i++) {
                    int v = edges[i][0], u_node = edges[i][1]-1, v_node = edges[i][2]-1;
                    for (int mask = (1<<MAX_N)-1; mask >=0; mask--) {
                        for (int a0 = 2*BOUND; a0 >=0; a0--) for (int a1 = 2*BOUND; a1 >=0; a1--) for (int a2 = 2*BOUND; a2 >=0; a2--) for (int a3 = 2*BOUND; a3 >=0; a3--) for (int a4 = 2*BOUND; a4 >=0; a4--) for (int a5 = 2*BOUND; a5 >=0; a5--) for (int a6 = 2*BOUND; a6 >=0; a6--) {
                            if (dp[mask][a0][a1][a2][a3][a4][a5][a6] == -1) continue;

                            int new_mask = mask | (1 << u_node);
                            int f[7] = {a0 - BOUND, a1 - BOUND, a2 - BOUND, a3 - BOUND, a4 - BOUND, a5 - BOUND, a6 - BOUND};

                            if (u_node < 7) {
                                f[u_node]++;
                            }
                            if (v_node < 7) {
                                f[v_node]--;
                            }

                            bool valid = true;
                            for (int i = 0; i < 7; i++) {
                                if (f[i] < -BOUND || f[i] > BOUND) {
                                    valid = false;
                                    break;
                                }
                            }
                            if (!valid) continue;

                            int n0 = get_index(f[0]), n1 = get_index(f[1]), n2 = get_index(f[2]), n3 = get_index(f[3]), n4 = get_index(f[4]), n5 = get_index(f[5]), n6 = get_index(f[6]);
                            int new_count = dp[mask][a0][a1][a2][a3][a4][a5][a6] + 1;
                            if (new_count > dp[new_mask][n0][n1][n2][n3][n4][n5][n6]) {
                                dp[new_mask][n0][n1][n2][n3][n4][n5][n6] = new_count;
                            }
                        }
                    }
                }
                if (dp[(1<<MAX_N)-1][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)][get_index(0)] >= K) {
                    ans = min(ans, range_val);
                    found = true;
                }
            }
        }
        if (ans == INF) {
            cout << -1 << endl;
        } else {
            cout << ans << endl;
        }
    } else {
        cout << ans << endl;
    }

    return 0;
}