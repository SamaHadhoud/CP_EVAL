#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    LL n, m;
    cin >> n >> m;

    vector<vector<pair<LL,LL>>> graph(n+1);
    vector<array<LL,3>> edges_list;
    for (LL i=0; i<m; i++) {
        LL u, v, t;
        cin >> u >> v >> t;
        graph[u].push_back({v, t});
        graph[v].push_back({u, t});
        edges_list.push_back({u, v, t});
    }

    vector<LL> d1(n+1, INF);
    vector<LL> parent_arr(n+1, -1);
    priority_queue< pair<LL,LL>, vector<pair<LL,LL>>, greater<pair<LL,LL>> > pq;

    d1[1] = 0;
    pq.push({0,1});
    while (!pq.empty()) {
        LL dist = pq.top().first;
        LL u = pq.top().second;
        pq.pop();
        if (dist != d1[u]) continue;
        for (auto &edge : graph[u]) {
            LL v = edge.first;
            LL w = edge.second;
            LL nd = dist + w;
            if (nd < d1[v]) {
                d1[v] = nd;
                parent_arr[v] = u;
                pq.push({nd, v});
            }
        }
    }

    if (d1[n] == INF) {
        cout << -1 << endl;
        return 0;
    }

    vector<LL> dn(n+1, INF);
    pq = priority_queue< pair<LL,LL>, vector<pair<LL,LL>>, greater<pair<LL,LL>> >();
    dn[n] = 0;
    pq.push({0, n});
    while (!pq.empty()) {
        LL dist = pq.top().first;
        LL u = pq.top().second;
        pq.pop();
        if (dist != dn[u]) continue;
        for (auto &edge : graph[u]) {
            LL v = edge.first;
            LL w = edge.second;
            LL nd = dist + w;
            if (nd < dn[v]) {
                dn[v] = nd;
                pq.push({nd, v});
            }
        }
    }

    set<LL> base_nodes;
    set<pair<LL,LL>> base_edges;
    base_nodes.insert(n);
    LL cur = n;
    while (cur != 1) {
        LL prev = parent_arr[cur];
        base_nodes.insert(prev);
        LL a = min(prev, cur);
        LL b = max(prev, cur);
        base_edges.insert({a, b});
        cur = prev;
    }
    base_nodes.insert(1);

    vector<vector<pair<LL,LL>>> graph_alt(n+1);
    for (auto &e : edges_list) {
        LL u = e[0], v = e[1], t = e[2];
        LL a = min(u,v);
        LL b = max(u,v);
        if (base_edges.find({a,b}) != base_edges.end()) {
            continue;
        }
        graph_alt[u].push_back({v, t});
        graph_alt[v].push_back({u, t});
    }

    vector<LL> dist_alt(n+1, INF);
    pq = priority_queue< pair<LL,LL>, vector<pair<LL,LL>>, greater<pair<LL,LL>> >();
    dist_alt[1] = 0;
    pq.push({0,1});
    while (!pq.empty()) {
        LL dist = pq.top().first;
        LL u = pq.top().second;
        pq.pop();
        if (dist != dist_alt[u]) continue;
        for (auto &edge : graph_alt[u]) {
            LL v = edge.first;
            LL w = edge.second;
            LL nd = dist + w;
            if (nd < dist_alt[v]) {
                dist_alt[v] = nd;
                pq.push({nd, v});
            }
        }
    }
    LL candidate_A_alt = INF;
    if (dist_alt[n] < INF) {
        candidate_A_alt = d1[n] + dist_alt[n];
    }

    LL candidate_B1 = INF;
    LL candidate_B4 = INF;
    for (auto &e : edges_list) {
        LL u = e[0], v = e[1], t = e[2];
        LL a = min(u,v);
        LL b = max(u,v);
        if (base_edges.find({a,b}) != base_edges.end()) {
            continue;
        }
        if (base_nodes.find(u) != base_nodes.end() && base_nodes.find(v) != base_nodes.end()) {
            LL seg = abs(d1[u] - d1[v]);
            LL cand = 2 * d1[n] - seg + t;
            if (cand < candidate_B1) {
                candidate_B1 = cand;
            }
        } else {
            LL option1 = d1[u] + dn[v] + t;
            LL option2 = d1[v] + dn[u] + t;
            LL cand = d1[n] + min(option1, option2);
            if (cand < candidate_B4) {
                candidate_B4 = cand;
            }
        }
    }

    LL ans = min({candidate_A_alt, candidate_B1, candidate_B4});
    if (ans >= INF) {
        cout << -1 << endl;
    } else {
        cout << ans << endl;
    }

    return 0;
}