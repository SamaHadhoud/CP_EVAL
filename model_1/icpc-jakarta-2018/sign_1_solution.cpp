#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

class Fenw {
    vector<int> tree;
    int n;
public:
    Fenw(int size) {
        n = size;
        tree.assign(n+1, 0);
    }
    void update(int idx, int delta) {
        if (idx <= 0) return;
        while (idx <= n) {
            tree[idx] += delta;
            idx += idx & -idx;
        }
    }
    int query(int idx) {
        if (idx <= 0) return 0;
        int sum = 0;
        while (idx) {
            sum += tree[idx];
            idx -= idx & -idx;
        }
        return sum;
    }
    int range_query(int l, int r) {
        if (l > r) return 0;
        if (l < 1) l = 1;
        if (r > n) r = n;
        return query(r) - query(l-1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, k;
    cin >> n >> k;
    vector<int> P(n);
    for (int i = 0; i < n; i++) {
        cin >> P[i];
    }

    vector<long long> base_prefix(n+1, 0);
    for (int i = 1; i <= n; i++) {
        if (P[i-1] == 0) {
            base_prefix[i] = base_prefix[i-1] - 1;
        } else {
            base_prefix[i] = base_prefix[i-1] + P[i-1];
        }
    }

    Fenw flip_tree(n);
    Fenw available_tree(n);
    set<int> available_set;
    vector<int> ans(n+1);

    for (int i = 1; i <= n; i++) {
        if (P[i-1] == 0) {
            ans[i] = -1;
            available_tree.update(i, 1);
            available_set.insert(i);
        } else {
            ans[i] = P[i-1];
        }
    }

    vector<vector<pair<int, int>>> constraints_by_B(n+1);
    for (int i = 0; i < k; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        constraints_by_B[b].push_back(make_pair(a, c));
    }

    for (int b = 1; b <= n; b++) {
        if (constraints_by_B[b].size() == 0) continue;
        sort(constraints_by_B[b].begin(), constraints_by_B[b].end(), [](const pair<int,int>& p1, const pair<int,int>& p2) {
            return p1.first > p2.first;
        });
    }

    for (int b = 1; b <= n; b++) {
        for (auto &con : constraints_by_B[b]) {
            int a = con.first;
            int c = con.second;
            long long base_interval = base_prefix[b] - base_prefix[a-1];
            long long already_flipped = flip_tree.range_query(a, b);
            long long total_current = base_interval + 2 * already_flipped;
            long long d = c - total_current;

            if (d <= 0) {
                continue;
            }

            long long t = (d + 1) / 2;
            long long count_avail = available_tree.range_query(a, b);
            if (count_avail < t) {
                cout << "Impossible" << endl;
                return 0;
            }

            while (t--) {
                auto it = available_set.upper_bound(b);
                if (it == available_set.begin()) {
                    cout << "Impossible" << endl;
                    return 0;
                }
                it--;
                if (*it < a) {
                    cout << "Impossible" << endl;
                    return 0;
                }
                int pos = *it;
                available_set.erase(it);
                available_tree.update(pos, -1);
                flip_tree.update(pos, 1);
                ans[pos] = 1;
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << " ";
        cout << ans[i];
    }
    cout << endl;

    return 0;
}