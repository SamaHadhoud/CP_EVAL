#include <iostream>
#include <vector>
#include <queue>
#include <cstdio>
#include <climits>
#include <tuple>
#include <algorithm>
using namespace std;

typedef long long ll;

int main() {
    int n, m;
    ll C;
    scanf("%d %d %lld", &n, &m, &C);
    vector<vector<tuple<int, ll, ll, ll>>> graph(n+1);

    for (int i = 0; i < m; i++) {
        int a, b;
        ll T_val, L_val, R_val;
        scanf("%d %d %lld %lld %lld", &a, &b, &T_val, &L_val, &R_val);
        graph[a].emplace_back(b, T_val, L_val, R_val);
        graph[b].emplace_back(a, T_val, L_val, R_val);
    }

    vector<ll> dist(n+1, LLONG_MAX);
    dist[1] = 0;
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    pq.push(make_pair(0, 1));

    while (!pq.empty()) {
        ll t = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (t != dist[u]) continue;

        for (const auto& edge : graph[u]) {
            int v = get<0>(edge);
            ll T_val = get<1>(edge);
            ll L_val = get<2>(edge);
            ll R_val = get<3>(edge);

            ll rem = t % C;
            ll start_time;
            if (rem <= R_val) {
                start_time = t + max(0LL, L_val - rem);
            } else {
                start_time = t + (C - rem) + L_val;
            }
            ll arrival = start_time + T_val;

            if (arrival < dist[v]) {
                dist[v] = arrival;
                pq.push(make_pair(arrival, v));
            }
        }
    }

    printf("%lld\n", dist[n]);
    return 0;
}