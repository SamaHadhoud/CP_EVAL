#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;
    vector<int> A(N+1);
    for (int i=1; i<=N; i++) {
        cin >> A[i];
    }
    vector<vector<int>> children(N+1);
    for (int i=2; i<=N; i++) {
        int p;
        cin >> p;
        children[p].push_back(i);
    }

    vector<int> min_node(N+1, 0);
    vector<int> max_node(N+1, 0);
    stack<pair<int, int>> dfs_stack;
    vector<int> visited(N+1, 0);
    vector<int> postorder;
    dfs_stack.push({1, 0});
    while (!dfs_stack.empty()) {
        int u = dfs_stack.top().first;
        int state = dfs_stack.top().second;
        dfs_stack.pop();
        if (state == 0) {
            dfs_stack.push({u, 1});
            for (int v : children[u]) {
                dfs_stack.push({v, 0});
            }
        } else {
            postorder.push_back(u);
        }
    }
    reverse(postorder.begin(), postorder.end());

    for (int u : postorder) {
        if (children[u].empty()) {
            min_node[u] = 1;
            max_node[u] = 1;
        } else {
            long long L = 0;
            long long R = 0;
            for (int v : children[u]) {
                L += min_node[v];
                R += max_node[v];
            }
            if (L >= A[u] + 1) {
                min_node[u] = L;
            } else {
                min_node[u] = L + 1;
            }
            max_node[u] = R + 1;
        }
    }

    if (K < min_node[1] || K > max_node[1]) {
        cout << -1 << endl;
        return 0;
    }

    vector<int> type(N+1, -1);
    vector<long long> k_val(N+1, 0);
    k_val[1] = K;

    queue<int> q;
    q.push(1);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        long long L = 0, R = 0;
        for (int v : children[u]) {
            L += min_node[v];
            R += max_node[v];
        }

        if (children[u].empty()) {
            if (k_val[u]-1 >= 0 && k_val[u]-1 <= 0) {
                type[u] = 0;
            } else if (k_val[u] >= max(0LL, (long long)A[u]+1) && k_val[u] <= 0) {
                type[u] = 1;
            } else {
                type[u] = 0;
            }
            continue;
        }

        if (k_val[u]-1 >= L && k_val[u]-1 <= R) {
            type[u] = 0;
            long long T = k_val[u] - 1;
            vector<pair<long long, int>> list;
            for (int v : children[u]) {
                list.push_back({max_node[v] - min_node[v], v});
            }
            sort(list.begin(), list.end(), greater<pair<long long, int>>());
            long long rem = T - L;
            for (auto& p : list) {
                int v = p.second;
                long long add = min(rem, (long long)(max_node[v] - min_node[v]));
                k_val[v] = min_node[v] + add;
                rem -= add;
                q.push(v);
            }
        } else if (k_val[u]-1 >= L && k_val[u]-1 <= min(R, (long long)A[u])) {
            type[u] = 1;
            long long T = k_val[u] - 1;
            vector<pair<long long, int>> list;
            for (int v : children[u]) {
                list.push_back({max_node[v] - min_node[v], v});
            }
            sort(list.begin(), list.end(), greater<pair<long long, int>>());
            long long rem = T - L;
            for (auto& p : list) {
                int v = p.second;
                long long add = min(rem, (long long)(max_node[v] - min_node[v]));
                k_val[v] = min_node[v] + add;
                rem -= add;
                q.push(v);
            }
        } else if (k_val[u] >= max(L, (long long)A[u]+1) && k_val[u] <= R) {
            type[u] = 1;
            long long T = k_val[u];
            vector<pair<long long, int>> list;
            for (int v : children[u]) {
                list.push_back({max_node[v] - min_node[v], v});
            }
            sort(list.begin(), list.end(), greater<pair<long long, int>>());
            long long rem = T - L;
            for (auto& p : list) {
                int v = p.second;
                long long add = min(rem, (long long)(max_node[v] - min_node[v]));
                k_val[v] = min_node[v] + add;
                rem -= add;
                q.push(v);
            }
        } else {
            type[u] = 0;
            long long T = k_val[u] - 1;
            vector<pair<long long, int>> list;
            for (int v : children[u]) {
                list.push_back({max_node[v] - min_node[v], v});
            }
            sort(list.begin(), list.end(), greater<pair<long long, int>>());
            long long rem = T - L;
            for (auto& p : list) {
                int v = p.second;
                long long add = min(rem, (long long)(max_node[v] - min_node[v]));
                k_val[v] = min_node[v] + add;
                rem -= add;
                q.push(v);
            }
        }
    }

    stack<pair<int, int>> output_stack;
    output_stack.push({1, 0});
    vector<int> result;

    while (!output_stack.empty()) {
        int u = output_stack.top().first;
        int state = output_stack.top().second;
        output_stack.pop();

        if (state == 0) {
            if (type[u] == 0) {
                result.push_back(u);
                vector<int>& c = children[u];
                for (auto it = c.rbegin(); it != c.rend(); ++it) {
                    output_stack.push({*it, 0});
                }
            } else {
                output_stack.push({u, 1});
                vector<int>& c = children[u];
                for (auto it = c.rbegin(); it != c.rend(); ++it) {
                    output_stack.push({*it, 0});
                }
            }
        } else {
            result.push_back(u);
        }
    }

    for (int i=0; i<result.size(); i++) {
        if (i>0) cout << " ";
        cout << result[i];
    }
    cout << endl;

    return 0;
}