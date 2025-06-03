#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m, K_val;
    cin >> n >> m >> K_val;

    vector<vector<int>> adj_out(n+1);
    vector<vector<int>> adj_in(n+1);
    set<pair<int, int>> edges;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj_out[u].push_back(v);
        adj_in[v].push_back(u);
        edges.insert({u, v});
    }

    vector<int> dp_in(n+1, 1);
    vector<int> dp_out(n+1, 1);

    for (int i = 1; i <= n; i++) {
        for (int j : adj_in[i]) {
            if (dp_in[j] + 1 > dp_in[i]) {
                dp_in[i] = dp_in[j] + 1;
            }
        }
    }

    for (int i = n; i >= 1; i--) {
        for (int j : adj_out[i]) {
            if (dp_out[j] + 1 > dp_out[i]) {
                dp_out[i] = dp_out[j] + 1;
            }
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int c, d;
        cin >> c >> d;
        if (edges.find({c, d}) != edges.end()) {
            cout << "accept\n";
        } else if (dp_in[c] + dp_out[d] >= K_val + 1) {
            cout << "reject\n";
        } else {
            cout << "accept\n";
            edges.insert({c, d});
            adj_out[c].push_back(d);
            adj_in[d].push_back(c);

            if (dp_in[c] + 1 > dp_in[d] && dp_in[c] + 1 <= K_val) {
                dp_in[d] = dp_in[c] + 1;
                queue<int> q_in;
                q_in.push(d);
                while (!q_in.empty()) {
                    int u = q_in.front();
                    q_in.pop();
                    for (int w : adj_out[u]) {
                        int cand = dp_in[u] + 1;
                        if (cand <= K_val && cand > dp_in[w]) {
                            dp_in[w] = cand;
                            q_in.push(w);
                        }
                    }
                }
            }

            if (dp_out[d] + 1 > dp_out[c] && dp_out[d] + 1 <= K_val) {
                dp_out[c] = dp_out[d] + 1;
                queue<int> q_out;
                q_out.push(c);
                while (!q_out.empty()) {
                    int u = q_out.front();
                    q_out.pop();
                    for (int w : adj_in[u]) {
                        int cand = dp_out[u] + 1;
                        if (cand <= K_val && cand > dp_out[w]) {
                            dp_out[w] = cand;
                            q_out.push(w);
                        }
                    }
                }
            }
        }
    }

    return 0;
}