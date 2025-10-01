#include <bits/stdc++.h>
using namespace std;

static inline unsigned long long pack(int u, int v) {
    if (u > v) swap(u, v);
    return (static_cast<unsigned long long>(u) << 32) | static_cast<unsigned long long>(v);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    vector<pair<int,int>> E1;
    E1.reserve(N - 1);
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        E1.emplace_back(u, v);
    }

    unordered_set<unsigned long long> S2;
    S2.reserve(static_cast<size_t>((N - 1) * 2));
    S2.max_load_factor(0.7f);
    for (int i = 0; i < N - 1; ++i) {
        int x, y;
        cin >> x >> y;
        S2.insert(pack(x, y));
    }

    int common = 0;
    for (const auto &e : E1) {
        if (S2.find(pack(e.first, e.second)) != S2.end()) ++common;
    }

    cout << (N - 1 - common) << '\n';
    return 0;
}