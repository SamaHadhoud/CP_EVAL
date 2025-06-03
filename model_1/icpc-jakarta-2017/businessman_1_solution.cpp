#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

typedef long long ll;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<ll> S(N+1);
    for (int i = 1; i <= N; i++) {
        cin >> S[i];
    }

    vector<vector<int>> graph(N+1);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> color(N+1, -1);
    vector<bool> visited(N+1, false);
    bool is_bipartite = true;
    queue<int> q;
    color[1] = 0;
    visited[1] = true;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                color[v] = color[u] ^ 1;
                q.push(v);
            } else {
                if (color[v] == color[u]) {
                    is_bipartite = false;
                }
            }
        }
    }

    if (!is_bipartite) {
        int Q;
        cin >> Q;
        while (Q--) {
            int A_i, B_i, C_i;
            cin >> A_i >> B_i >> C_i;
            if (A_i == 1) {
                cout << 0 << '\n';
            } else {
            }
        }
    } else {
        set<pair<ll, int>> sorted_set;
        vector<ll> current_value(N+1);
        for (int i = 1; i <= N; i++) {
            current_value[i] = S[i];
            sorted_set.insert({S[i], i});
        }

        multiset<ll> gap_set;
        auto it = sorted_set.begin();
        auto next_it = next(it);
        while (next_it != sorted_set.end()) {
            if (color[it->second] != color[next_it->second]) {
                gap_set.insert(next_it->first - it->first);
            }
            it++;
            next_it++;
        }

        int Q;
        cin >> Q;
        while (Q--) {
            int A_i, B_i, C_i;
            cin >> A_i >> B_i >> C_i;
            if (A_i == 0) {
                ll old_val = current_value[B_i];
                auto it_old = sorted_set.find({old_val, B_i});
                if (it_old == sorted_set.end()) {
                    continue;
                }

                auto prev_it = (it_old != sorted_set.begin()) ? prev(it_old) : sorted_set.end();
                auto next_it = next(it_old);
                if (next_it == sorted_set.end()) next_it = sorted_set.end();

                if (prev_it != sorted_set.end()) {
                    if (color[prev_it->second] != color[B_i]) {
                        ll gap_val = it_old->first - prev_it->first;
                        auto it_gap = gap_set.find(gap_val);
                        if (it_gap != gap_set.end()) {
                            gap_set.erase(it_gap);
                        }
                    }
                }
                if (next_it != sorted_set.end()) {
                    if (color[B_i] != color[next_it->second]) {
                        ll gap_val = next_it->first - it_old->first;
                        auto it_gap = gap_set.find(gap_val);
                        if (it_gap != gap_set.end()) {
                            gap_set.erase(it_gap);
                        }
                    }
                }
                if (prev_it != sorted_set.end() && next_it != sorted_set.end()) {
                    if (color[prev_it->second] != color[next_it->second]) {
                        ll gap_val = next_it->first - prev_it->first;
                        gap_set.insert(gap_val);
                    }
                }

                sorted_set.erase(it_old);

                current_value[B_i] = C_i;
                auto it_new = sorted_set.insert({C_i, B_i}).first;

                auto prev_new = (it_new != sorted_set.begin()) ? prev(it_new) : sorted_set.end();
                auto next_new = next(it_new);
                if (next_new == sorted_set.end()) next_new = sorted_set.end();

                if (prev_new != sorted_set.end() && next_new != sorted_set.end()) {
                    if (color[prev_new->second] != color[next_new->second]) {
                        ll gap_val = next_new->first - prev_new->first;
                        auto it_gap = gap_set.find(gap_val);
                        if (it_gap != gap_set.end()) {
                            gap_set.erase(it_gap);
                        }
                    }
                }

                if (prev_new != sorted_set.end()) {
                    if (color[prev_new->second] != color[B_i]) {
                        ll gap_val = it_new->first - prev_new->first;
                        gap_set.insert(gap_val);
                    }
                }
                if (next_new != sorted_set.end()) {
                    if (color[B_i] != color[next_new->second]) {
                        ll gap_val = next_new->first - it_new->first;
                        gap_set.insert(gap_val);
                    }
                }

            } else {
                if (color[B_i] == color[C_i]) {
                    cout << 0 << '\n';
                } else {
                    if (gap_set.empty()) {
                        cout << 0 << '\n';
                    } else {
                        cout << *gap_set.begin() << '\n';
                    }
                }
            }
        }
    }

    return 0;
}