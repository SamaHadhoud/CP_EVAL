#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef long long ll;
const ll mod1 = 1000000007;
const ll mod2 = 1000000009;
const ll base = 131;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<vector<int>> graph(n);
    vector<int> deg(n, 0);
    for (int i = 0; i < n-1; i++) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        graph[a].push_back(b);
        graph[b].push_back(a);
        deg[a]++;
        deg[b]++;
    }

    int total_nodes_minus_one = n - 1;
    int ans = -1;

    for (int u = 0; u < n; u++) {
        int k = deg[u];
        if (k < 2 || total_nodes_minus_one % k != 0) {
            continue;
        }
        int s = total_nodes_minus_one / k;

        vector<bool> visited(n, false);
        vector<bool> in_comp(n, false);
        vector<vector<int>> comps_vec;
        visited[u] = true;

        for (int v : graph[u]) {
            if (visited[v]) continue;
            vector<int> comp_vec;
            queue<int> q;
            q.push(v);
            visited[v] = true;
            comp_vec.push_back(v);
            in_comp[v] = true;
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                for (int neighbor : graph[node]) {
                    if (neighbor == u) continue;
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        comp_vec.push_back(neighbor);
                        in_comp[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
            comps_vec.push_back(comp_vec);
        }

        if (comps_vec.size() != k) {
            continue;
        }

        bool valid_size = true;
        for (const vector<int>& comp : comps_vec) {
            if (comp.size() != s) {
                valid_size = false;
                break;
            }
        }
        if (!valid_size) {
            continue;
        }

        vector<pair<ll, ll>> comp_hashes;
        for (vector<int> comp : comps_vec) {
            vector<int> deg_in_comp(n, 0);
            for (int node : comp) {
                int cnt = 0;
                for (int neighbor : graph[node]) {
                    if (neighbor != u && in_comp[neighbor]) {
                        cnt++;
                    }
                }
                deg_in_comp[node] = cnt;
            }

            queue<int> leaves;
            vector<int> deg_work = deg_in_comp;
            int total_comp = comp.size();
            for (int node : comp) {
                if (deg_work[node] == 1) {
                    leaves.push(node);
                }
            }

            while (total_comp > 2) {
                int num_leaves = leaves.size();
                total_comp -= num_leaves;
                for (int i = 0; i < num_leaves; i++) {
                    int node = leaves.front();
                    leaves.pop();
                    for (int neighbor : graph[node]) {
                        if (neighbor == u) continue;
                        if (in_comp[neighbor] && deg_work[neighbor] > 1) {
                            deg_work[neighbor]--;
                            if (deg_work[neighbor] == 1) {
                                leaves.push(neighbor);
                            }
                        }
                    }
                }
            }

            vector<int> centers;
            while (!leaves.empty()) {
                centers.push_back(leaves.front());
                leaves.pop();
            }

            vector<pair<ll, ll>> center_hashes;
            for (int center : centers) {
                vector<int> parent(n, -1);
                vector<vector<int>> children(n);
                vector<int> order;
                queue<int> q;
                q.push(center);
                parent[center] = -1;
                while (!q.empty()) {
                    int node = q.front();
                    q.pop();
                    order.push_back(node);
                    for (int neighbor : graph[node]) {
                        if (neighbor == u) continue;
                        if (!in_comp[neighbor]) continue;
                        if (neighbor == parent[node]) continue;
                        parent[neighbor] = node;
                        children[node].push_back(neighbor);
                        q.push(neighbor);
                    }
                }

                vector<ll> h1(n, 1);
                vector<ll> h2(n, 1);
                for (int i = order.size() - 1; i >= 0; i--) {
                    int node = order[i];
                    vector<pair<ll, ll>> child_hashes;
                    for (int child : children[node]) {
                        child_hashes.push_back({h1[child], h2[child]});
                    }
                    sort(child_hashes.begin(), child_hashes.end());
                    for (auto& p : child_hashes) {
                        h1[node] = (h1[node] * base + p.first) % mod1;
                        h2[node] = (h2[node] * base + p.second) % mod2;
                    }
                }
                center_hashes.push_back({h1[center], h2[center]});
            }

            pair<ll, ll> comp_hash = center_hashes[0];
            for (int i = 1; i < center_hashes.size(); i++) {
                if (center_hashes[i] < comp_hash) {
                    comp_hash = center_hashes[i];
                }
            }
            comp_hashes.push_back(comp_hash);

            for (int node : comp) {
                in_comp[node] = false;
            }
        }

        bool all_same = true;
        for (int i = 1; i < comp_hashes.size(); i++) {
            if (comp_hashes[i] != comp_hashes[0]) {
                all_same = false;
                break;
            }
        }
        if (all_same) {
            if (k > ans) {
                ans = k;
            }
        }
    }

    if (ans == -1) {
        cout << -1 << endl;
    } else {
        cout << ans << endl;
    }

    return 0;
}