#include <bits/stdc++.h>
using namespace std;

static const long long NEG_INF = (long long)-9e18;
static const int MAXS = 55;

inline int phi_penalty(int L, int a, int b) {
    int s = a + b;
    if ((L & 1) == 0) { // L even
        return (s >= 1) ? 1 : 0;
    } else { // L odd
        return (s == 2) ? 1 : 0;
    }
}

long long singles_gain(long long D, int O, long long K, long long G1, long long G2) {
    long long best = 0;
    long long delta = 2 * G1 - G2;
    if (delta < 0) delta = 0;
    int max_s1 = (int)min<long long>(K, O);
    for (int s1 = 0; s1 <= max_s1; ++s1) {
        long long remK = K - s1;
        long long break_cnt = min(D, remK / 2);
        long long val = G1 * s1 + delta * break_cnt;
        if (val > best) best = val;
    }
    return best;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    long long K, G1, G2, G3;
    string S;
    if (!(cin >> N >> K >> G1 >> G2 >> G3)) return 0;
    cin >> S;

    // Build segments of '.' between '#'
    vector<int> L;
    L.reserve(60);
    int cnt = 0, m = 0;
    for (char c : S) {
        if (c == '.') {
            ++cnt;
        } else { // '#'
            L.push_back(cnt);
            cnt = 0;
            ++m;
        }
    }
    L.push_back(cnt); // last segment
    int segCnt = (int)L.size(); // = m + 1

    // Constant S = sum floor(L[i]/2)
    long long S_const = 0;
    for (int x : L) S_const += x / 2;
    long long base = G2 * S_const;

    // Feasibility of Type-3 at each rock j (0..m-1)
    vector<char> feasible(m, false);
    for (int j = 0; j < m; ++j) {
        feasible[j] = (L[j] >= 1 && L[j + 1] >= 1);
    }

    // DP arrays: dp_prev[prev][o][p], dp_cur[...]
    static long long dp_prev[2][MAXS][MAXS];
    static long long dp_cur[2][MAXS][MAXS];
    for (int a = 0; a < 2; ++a)
        for (int o = 0; o <= segCnt; ++o)
            for (int p = 0; p <= segCnt; ++p)
                dp_prev[a][o][p] = NEG_INF;

    dp_prev[0][0][0] = 0; // start before first rock

    for (int j = 0; j < m; ++j) {
        for (int a = 0; a < 2; ++a)
            for (int o = 0; o <= segCnt; ++o)
                for (int p = 0; p <= segCnt; ++p)
                    dp_cur[a][o][p] = NEG_INF;

        for (int prev = 0; prev <= 1; ++prev) {
            for (int o = 0; o <= segCnt; ++o) {
                for (int p = 0; p <= segCnt; ++p) {
                    long long v = dp_prev[prev][o][p];
                    if (v <= NEG_INF / 2) continue;

                    for (int x = 0; x <= 1; ++x) {
                        if (x == 1 && !feasible[j]) continue;
                        if (L[j] == 1 && prev == 1 && x == 1) continue; // overlap on length-1 segment

                        int o_inc = (L[j] + prev + x) & 1;
                        int pen = phi_penalty(L[j], prev, x);

                        int o2 = o + o_inc;
                        int p2 = p + pen;
                        long long v2 = v + G3 * x - G2 * pen;

                        if (o2 <= segCnt && p2 <= segCnt) {
                            long long &ref = dp_cur[x][o2][p2];
                            if (v2 > ref) ref = v2;
                        }
                    }
                }
            }
        }
        // swap
        for (int a = 0; a < 2; ++a)
            for (int o = 0; o <= segCnt; ++o)
                for (int p = 0; p <= segCnt; ++p)
                    dp_prev[a][o][p] = dp_cur[a][o][p];
    }

    // Finalize the last segment (index m)
    long long best = 0; // It's possible to place nothing and get 0
    for (int prev = 0; prev <= 1; ++prev) {
        for (int o = 0; o <= segCnt; ++o) {
            for (int p = 0; p <= segCnt; ++p) {
                long long v = dp_prev[prev][o][p];
                if (v <= NEG_INF / 2) continue;

                int o_inc = (L[m] + prev + 0) & 1;
                int pen = phi_penalty(L[m], prev, 0);
                int o2 = o + o_inc;
                int p2 = p + pen;

                if (o2 > segCnt || p2 > segCnt) continue;

                long long v2 = v - G2 * pen;
                long long D = S_const - p2;

                long long add = singles_gain(D, o2, K, G1, G2);
                long long total = base + v2 + add;
                if (total > best) best = total;
            }
        }
    }

    cout << best << '\n';
    return 0;
}