#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<vector<int>> adj(n+1);
    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> depth(n+1, 0);
    vector<int> sign_vec(n+1, 0);
    vector<int> parent(n+1, 0);
    queue<int> q;
    vector<bool> visited(n+1, false);
    q.push(1);
    visited[1] = true;
    depth[1] = 0;
    sign_vec[1] = 1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (visited[v]) continue;
            visited[v] = true;
            depth[v] = depth[u] + 1;
            sign_vec[v] = (depth[v] % 2 == 0) ? 1 : -1;
            parent[v] = u;
            q.push(v);
        }
    }

    vector<bool> active(n+1, true);
    vector<long long> deg(n+1, 0);
    vector<int> comp_id(n+1, 1);
    int comp_counter = 1;
    vector<vector<int>> comp_nodes(2 * 100000 + 1000);
    comp_nodes[1] = vector<int>();
    for (int i = 1; i <= n; i++) {
        comp_nodes[1].push_back(i);
    }

    vector<int> last_vis(n+1, 0);
    int last_vis_token = 0;

    int Q;
    cin >> Q;
    vector<long long> energy_ans;
    for (int i_op = 0; i_op < Q; i_op++) {
        int op_type;
        cin >> op_type;
        if (op_type == 1) {
            int x;
            cin >> x;
            active[x] = false;
            int cid = comp_id[x];
            last_vis_token++;
            vector<int> nodes_in_component = comp_nodes[cid];
            for (int node : nodes_in_component) {
                if (node == x) continue;
                if (!active[node]) continue;
                if (last_vis[node] == last_vis_token) continue;
                comp_counter++;
                queue<int> q_bfs;
                q_bfs.push(node);
                last_vis[node] = last_vis_token;
                vector<int> new_comp;
                new_comp.push_back(node);
                while (!q_bfs.empty()) {
                    int u = q_bfs.front(); q_bfs.pop();
                    for (int v : adj[u]) {
                        if (v == x) continue;
                        if (!active[v]) continue;
                        if (last_vis[v] == last_vis_token) continue;
                        if (comp_id[v] != cid) continue;
                        last_vis[v] = last_vis_token;
                        q_bfs.push(v);
                        new_comp.push_back(v);
                    }
                }
                comp_nodes[comp_counter] = new_comp;
                for (int node_in_comp : new_comp) {
                    comp_id[node_in_comp] = comp_counter;
                }
            }
        } else if (op_type == 2) {
            int x;
            cin >> x;
            active[x] = true;
            comp_counter++;
            int new_cid = comp_counter;
            comp_id[x] = new_cid;
            comp_nodes[new_cid] = {x};
            set<int> seen_comps;
            for (int y : adj[x]) {
                if (!active[y]) continue;
                int cid_y = comp_id[y];
                if (seen_comps.find(cid_y) != seen_comps.end()) continue;
                seen_comps.insert(cid_y);
                for (int node : comp_nodes[cid_y]) {
                    comp_id[node] = new_cid;
                    comp_nodes[new_cid].push_back(node);
                }
                comp_nodes[cid_y].clear();
            }
        } else if (op_type == 3) {
            int x, alpha;
            cin >> x >> alpha;
            int cid = comp_id[x];
            long long energy = (long long)comp_nodes[cid].size() * (long long)alpha;
            energy_ans.push_back(energy);
            for (int node : comp_nodes[cid]) {
                long long add_val = (long long)alpha * (long long)sign_vec[x] * (long long)sign_vec[node];
                deg[node] = (deg[node] + add_val) % 360;
                if (deg[node] < 0) deg[node] += 360;
            }
        }
    }

    for (long long e : energy_ans) {
        cout << e << '\n';
    }

    long long total_deg = 0;
    for (int i = 1; i <= n; i++) {
        total_deg += deg[i];
    }
    cout << total_deg << '\n';

    return 0;
}