```cpp
#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<long long> bit;
    Fenwick() {}
    Fenwick(int n): n(n), bit(n + 1, 0) {}
    void add(int idx, long long delta) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += delta;
    }
    long long sumPrefix(int idx) const {
        long long s = 0;
        for (; idx > 0; idx -= idx & -idx) s += bit[idx];
        return s;
    }
    long long sumAll() const { return sumPrefix(n); }
    // Smallest index idx such that prefix sum >= k (1-based). Assumes 1 <= k <= sumAll()
    int kth(long long k) const {
        int idx = 0;
        int pw = 1;
        while ((pw << 1) <= n) pw <<= 1;
        for (int step = pw; step; step >>= 1) {
            int next = idx + step;
            if (next <= n && bit[next] < k) {
                idx = next;
                k -= bit[next];
            }
        }
        return idx + 1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<int> A(N), B(N);
    int maxA = 0, maxB = 0;
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        maxA = max(maxA, A[i]);
    }
    for (int i = 0; i < N; ++i) {
        cin >> B[i];
        maxB = max(maxB, B[i]);
    }

    vector<vector<int>> bucket(maxA + 2);
    vector<int> C(maxA + 2, 0);
    vector<long long> S(maxA + 2, 0);
    long long total = 0;
    for (int i = 0; i < N; ++i) {
        bucket[A[i]].push_back(B[i]);
        C[A[i]]++;
        S[A[i]] += B[i];
        total += B[i];
    }

    Fenwick cntBIT(maxB), sumBIT(maxB);
    long long bestSaved = 0;
    long long curCnt = 0, curSum = 0;

    auto topKlargestSum = [&](int K) -> long long {
        if (K <= 0) return 0;
        if (K >= curCnt) return curSum;
        long long t = curCnt - K; // exclude t smallest
        int c = cntBIT.kth(t);    // cost value at which exclusion boundary lies
        long long countBelow = cntBIT.sumPrefix(c - 1);
        long long sumBelow = sumBIT.sumPrefix(c - 1);
        long long takeAtC = t - countBelow;
        long long sumSmallest = sumBelow + takeAtC * 1LL * c;
        return curSum - sumSmallest;
    };

    for (int v = 1; v <= maxA; ++v) {
        if (C[v] > 0) {
            int K = (int)min<long long>(C[v] - 1, curCnt);
            long long topK = topKlargestSum(K);
            bestSaved = max(bestSaved, S[v] + topK);
        }
        // Insert all elements with value v into the multiset M of (Ai < next v)
        for (int c : bucket[v]) {
            cntBIT.add(c, 1);
            sumBIT.add(c, c);
            curCnt += 1;
            curSum += c;
        }
    }

    long long answer = total - bestSaved;
    cout << answer << '\n';
    return 0;
}
```