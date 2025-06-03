#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <tuple>
using namespace std;

const int N = 100010;
unordered_set<int> g[N];
bool inQ[N];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].insert(v);
        g[v].insert(u);
    }

    queue<int> q;
    vector<tuple<int, int, int>> ans;

    for (int i = 1; i <= n; i++) {
        if (g[i].size() >= 2) {
            q.push(i);
            inQ[i] = true;
        }
    }

    while (!q.empty()) {
        int v = q.front(); 
        q.pop();
        inQ[v] = false;

        if (g[v].size() < 2) 
            continue;

        int a = *g[v].begin();
        g[v].erase(a);
        g[a].erase(v);

        int b = *g[v].begin();
        g[v].erase(b);
        g[b].erase(v);

        ans.push_back(make_tuple(a, v, b));

        vector<int> candidates = {a, b, v};
        for (int u : candidates) {
            if (g[u].size() >= 2 && !inQ[u]) {
                inQ[u] = true;
                q.push(u);
            }
        }
    }

    cout << ans.size() << "\n";
    for (auto tpl : ans) {
        cout << get<0>(tpl) << " " << get<1>(tpl) << " " << get<2>(tpl) << "\n";
    }
}