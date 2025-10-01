#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;

    vector<pair<long long, long long>> v;
    v.reserve(N);

    long long best_single = 0; // stores Li*Wi

    for (int i = 0; i < N; ++i) {
        long long L, W;
        cin >> L >> W;
        best_single = max(best_single, L * W);
        long long P = max(L, W);
        long long Q = min(L, W);
        v.emplace_back(P, Q);
    }

    sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first > b.first; // sort by P desc
        return a.second > b.second;                       // tie-break by Q desc
    });

    long long best_pair = 0; // best over two lands: min(Pi,Pj)*min(Qi,Qj)
    if (N >= 2) {
        long long M = v[0].second; // max Q in prefix
        for (int t = 1; t < N; ++t) {
            long long P = v[t].first;
            long long Q = v[t].second;
            long long candidate = P * min(Q, M);
            if (candidate > best_pair) best_pair = candidate;
            if (Q > M) M = Q;
        }
    }

    // Compare doubled areas to avoid floating point: 
    // same-land best is best_single/2, two-lands best is best_pair
    long long double_ans = max(best_single, best_pair * 2);

    if (double_ans % 2 == 0) {
        cout << (double_ans / 2) << ".0\n";
    } else {
        cout << (double_ans / 2) << ".5\n";
    }
    return 0;
}