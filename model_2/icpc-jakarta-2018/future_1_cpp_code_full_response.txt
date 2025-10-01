```cpp
#include <bits/stdc++.h>
using namespace std;

static vector<string> buildDistinctSubseq(const string& s) {
    int L = (int)s.size();
    vector<string> v;
    v.reserve((1 << L) - 1);
    for (int mask = 1; mask < (1 << L); ++mask) {
        string t;
        for (int i = 0; i < L; ++i) {
            if (mask & (1 << i)) t.push_back(s[i]);
        }
        v.push_back(move(t));
    }
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    return v;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<string> S(N);
    for (int i = 0; i < N; ++i) cin >> S[i];

    // Layer 1
    vector<string> A = buildDistinctSubseq(S[0]);
    vector<int> dp(A.size());
    for (size_t i = 0; i < A.size(); ++i) dp[i] = (int)A[i].size();

    // Layers 2..N
    for (int i = 1; i < N; ++i) {
        vector<string> B = buildDistinctSubseq(S[i]);
        vector<int> dp2(B.size(), -1);

        // Two-pointer sweep: O(|A| + |B|)
        size_t k = 0;
        int curBest = -1;
        for (size_t j = 0; j < B.size(); ++j) {
            while (k < A.size() && A[k] < B[j]) {
                if (dp[k] > curBest) curBest = dp[k];
                ++k;
            }
            if (curBest >= 0) dp2[j] = curBest + (int)B[j].size();
        }

        A.swap(B);
        dp.swap(dp2);
    }

    int ans = -1;
    for (int v : dp) ans = max(ans, v);
    cout << ans << '\n';
    return 0;
}
```