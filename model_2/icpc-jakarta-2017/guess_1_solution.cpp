#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N;
    int K;
    if (!(cin >> N >> K)) return 0;
    string s;
    cin >> s;
    int M = (int)s.size();

    const long long INF = 4000000000000000000LL; // 4e18 > 1e18

    vector<long long> ans;
    ans.reserve(M);

    size_t eqpos = s.find('=');

    // If '=' appears but not at the end, it's invalid per problem statement
    if (eqpos != string::npos && eqpos != s.size() - 1) {
        cout << -1 << '\n';
        return 0;
    }

    if (eqpos == string::npos) {
        // No '=' case
        if ((long long)M > N - 1) {
            cout << -1 << '\n';
            return 0;
        }
        long long L = 1, R = N;
        for (int i = 0; i < M; ++i) {
            if (s[i] == '<') {
                ans.push_back(R);
                --R;
            } else { // '>'
                ans.push_back(L);
                ++L;
            }
        }
        for (int i = 0; i < M; ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
        return 0;
    } else {
        // Ends with '='
        int t = M - 1; // number of strict answers before final '='
        // Backward feasibility
        vector<long long> need_low(t + 1), need_high(t + 1);
        need_low[t] = need_high[t] = 1;
        for (int i = t - 1; i >= 0; --i) {
            long long a = need_low[i + 1], b = need_high[i + 1];
            if (s[i] == '<') {
                long long low = a + 1;
                long long high = 2 * b + 1;
                if (low > INF) low = INF;
                if (high > INF) high = INF;
                need_low[i] = low;
                need_high[i] = high;
            } else { // '>'
                long long low = a + 1;
                long long high = 2 * b;
                if (low > INF) low = INF;
                if (high > INF) high = INF;
                need_low[i] = low;
                need_high[i] = high;
            }
        }

        if (N < need_low[0] || N > need_high[0]) {
            cout << -1 << '\n';
            return 0;
        }

        // Forward construction
        long long L = 1, R = N, cur_len = N;
        for (int i = 0; i < t; ++i) {
            // Allowed next lengths from current length and direction
            long long loA, hiA;
            if (s[i] == '<') {
                loA = cur_len / 2;          // ceil((cur_len - 1)/2)
                hiA = cur_len - 1;
            } else { // '>'
                loA = (cur_len + 1) / 2;    // floor((cur_len - 1)/2) + 1
                hiA = cur_len - 1;
            }

            long long loB = need_low[i + 1], hiB = need_high[i + 1];
            long long lo = max(loA, loB);
            long long hi = min(hiA, hiB);
            if (lo > hi) { // Should not happen if backward feasibility is correct
                cout << -1 << '\n';
                return 0;
            }
            long long next_len = hi; // keep as large as possible

            if (s[i] == '<') {
                long long Ai = L + next_len; // left_size = next_len
                ans.push_back(Ai);
                R = Ai - 1;
            } else { // '>'
                long long Ai = R - next_len; // right_size = next_len
                ans.push_back(Ai);
                L = Ai + 1;
            }
            cur_len = next_len;
        }
        // Final '=' guess (L == R)
        ans.push_back(L);

        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }

    return 0;
}