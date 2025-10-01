#include <bits/stdc++.h>
using namespace std;

using U64 = unsigned long long;

struct PairHash {
    size_t operator()(const pair<U64, U64>& p) const noexcept {
        return (size_t)(p.first ^ (p.second * 0x9e3779b97f4a7c15ULL));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    if (!(cin >> N >> M >> K)) return 0;
    vector<int> A(M);
    for (int i = 0; i < M; ++i) cin >> A[i];

    sort(A.begin(), A.end());
    vector<int> D = A;
    sort(D.begin(), D.end(), greater<int>()); // descending order
    int minD = A[0];

    // Special case N == 1: just output any K distinct digits from A
    if (N == 1) {
        string S;
        S.reserve(K);
        for (int i = 0; i < K; ++i) S.push_back(char('0' + A[i]));
        cout << S << '\n';
        return 0;
    }

    // Two 64-bit hashes to virtually eliminate collisions
    const U64 B1 = 1315423911ULL;                 // odd
    const U64 B2 = 0x9e3779b97f4a7c15ULL;         // odd

    U64 powB1 = 1, powB2 = 1; // B^(N-1)
    for (int i = 0; i < N - 1; ++i) {
        powB1 *= B1;
        powB2 *= B2;
    }

    // Initial suffix: N-1 copies of minD
    string S;
    S.reserve((size_t)N + K - 1);
    S.append(N - 1, char('0' + minD));

    // Hash of the current suffix (length N-1), high-degree for older chars
    U64 h1 = 0, h2 = 0;
    for (int i = 0; i < N - 1; ++i) {
        h1 = h1 * B1 + (U64)minD;
        h2 = h2 * B2 + (U64)minD;
    }

    unordered_set<pair<U64, U64>, PairHash> seen;
    seen.reserve((size_t)K * 2);

    for (int step = 0; step < K; ++step) {
        bool appended = false;
        // First digit of the current (N-1)-suffix in S
        int idxFirst = (int)S.size() - (N - 1);
        int firstDigit = S[idxFirst] - '0';

        for (int d : D) {
            U64 w1 = h1 * B1 + (U64)d;
            U64 w2 = h2 * B2 + (U64)d;
            auto key = make_pair(w1, w2);
            if (seen.find(key) == seen.end()) {
                seen.insert(key);
                S.push_back(char('0' + d));
                // roll suffix hash: new suffix is old suffix shifted left + d
                h1 = w1 - (U64)firstDigit * powB1;
                h2 = w2 - (U64)firstDigit * powB2;
                appended = true;
                break;
            }
        }
        // The greedy construction is guaranteed not to get stuck for K <= M^N
        // due to de Bruijn properties; assert for safety in debug scenarios.
        if (!appended) {
            // Should never happen under valid constraints; fallback to avoid runtime issues.
            // Append any digit (keeps length) though it might break optimality; but constraints guarantee this path unused.
            S.push_back(char('0' + D.back()));
            // Update hashes consistently (though correctness relies on appended=true always).
            U64 w1 = h1 * B1 + (U64)D.back();
            U64 w2 = h2 * B2 + (U64)D.back();
            h1 = w1 - (U64)firstDigit * powB1;
            h2 = w2 - (U64)firstDigit * powB2;
        }
    }

    cout << S << '\n';
    return 0;
}