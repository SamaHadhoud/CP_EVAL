#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <queue>
#include <map>
using namespace std;
typedef long long ll;

struct WaveletNode {
    int lo, hi;
    vector<int> C;
    WaveletNode *left, *right;

    WaveletNode(vector<int> arr, int lo_val, int hi_val) : lo(lo_val), hi(hi_val), left(nullptr), right(nullptr) {
        if (arr.empty() || lo == hi) {
            return;
        }
        int mid = (lo + hi) / 2;
        C.clear();
        C.push_back(0);
        vector<int> left_arr, right_arr;
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i] <= mid) {
                left_arr.push_back(arr[i]);
                C.push_back(C.back() + 1);
            } else {
                right_arr.push_back(arr[i]);
                C.push_back(C.back());
            }
        }
        if (!left_arr.empty()) {
            left = new WaveletNode(left_arr, lo, mid);
        }
        if (!right_arr.empty()) {
            right = new WaveletNode(right_arr, mid+1, hi);
        }
    }

    int query(int l, int r, int x) {
        if (l > r) {
            return 0;
        }
        if (lo == hi) {
            if (lo <= x) {
                return r - l + 1;
            } else {
                return 0;
            }
        }
        int mid = (lo + hi) / 2;
        if (x <= mid) {
            if (left) {
                int new_l = C[l];
                int new_r = C[r+1] - 1;
                return left->query(new_l, new_r, x);
            } else {
                return 0;
            }
        } else {
            int left_count = C[r+1] - C[l];
            int right_count = 0;
            if (right) {
                int new_l = l - C[l];
                int new_r = r - C[r+1];
                right_count = right->query(new_l, new_r, x);
            }
            return left_count + right_count;
        }
    }

    ~WaveletNode() {
        delete left;
        delete right;
    }
};

struct WaveletTree {
    WaveletNode *root;
    int min_val, max_val;

    WaveletTree(vector<int> arr, int lo, int hi) {
        root = new WaveletNode(arr, lo, hi);
        min_val = lo;
        max_val = hi;
    }

    int query(int l, int r, int x) {
        if (l > r || x < min_val) return 0;
        if (x >= max_val) {
            return r - l + 1;
        }
        return root->query(l, r, x);
    }

    ~WaveletTree() {
        delete root;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> p_arr(n);
    vector<int> parent_arr(n+1, 0);
    vector<vector<int>> children(n+1);

    int root = -1;
    for (int i = 0; i < n; i++) {
        cin >> p_arr[i];
        parent_arr[i+1] = p_arr[i];
        if (p_arr[i] == 0) {
            root = i+1;
        } else {
            children[p_arr[i]].push_back(i+1);
        }
    }

    bool is_chain = true;
    if (n > 1) {
        if (children[root].size() != 1) {
            is_chain = false;
        }
        for (int i = 1; i <= n; i++) {
            if (i != root && children[i].size() > 1) {
                is_chain = false;
                break;
            }
        }
    } else {
        is_chain = true;
    }

    vector<int> in_time(n+1, -1), out_time(n+1, -1);
    vector<int> A;
    if (!is_chain) {
        vector<int> ptr(n+1, 0);
        stack<int> st;
        st.push(root);
        in_time[root] = 0;
        A.resize(n);
        A[0] = root;
        int timer = 1;
        while (!st.empty()) {
            int u = st.top();
            if (ptr[u] < children[u].size()) {
                int v = children[u][ptr[u]++];
                in_time[v] = timer;
                A[timer] = v;
                timer++;
                st.push(v);
            } else {
                out_time[u] = timer - 1;
                st.pop();
            }
        }
    }

    WaveletTree *wt = nullptr;
    if (!is_chain) {
        wt = new WaveletTree(A, 1, n);
    }

    map<int, vector<int>> path_cache;

    for (int i = 0; i < q; i++) {
        ll k;
        cin >> k;
        k--;

        ll x0 = k / n + 1;
        ll r = k % n + 1;

        if (is_chain) {
            ll inner_val;
            if (r <= x0 - 1) {
                inner_val = (r - 1) * (n + 1LL);
            } else {
                inner_val = (x0 - 1) * n + (r - 1);
            }
            ll ans = (x0 - 1) * n * 1LL * n + inner_val;
            cout << ans << '\n';
        } else {
            if (path_cache.find(x0) == path_cache.end()) {
                vector<int> path;
                int cur = x0;
                while (cur != 0) {
                    path.push_back(cur);
                    cur = parent_arr[cur];
                }
                reverse(path.begin(), path.end());
                path_cache[x0] = path;
            }
            const vector<int>& path = path_cache[x0];
            ll low_val = 0;
            ll high_val = (ll)n * (ll)n - 1;

            auto count_in_row = [&](ll V) -> ll {
                ll total = 0;
                for (int idx = 0; idx < path.size(); idx++) {
                    int z = path[idx];
                    ll base = (ll)(z - 1) * (ll)n;
                    if (base > V) {
                        break;
                    }
                    ll rem = V - base;
                    int Y0 = min(rem + 1, (ll)n);

                    if (idx < path.size() - 1) {
                        int next_node = path[idx+1];
                        int L1 = in_time[z];
                        int R1 = in_time[next_node] - 1;
                        ll count1 = 0;
                        if (L1 <= R1) {
                            count1 = wt->query(L1, R1, Y0);
                        }

                        int L2 = out_time[next_node] + 1;
                        int R2 = out_time[z];
                        ll count2 = 0;
                        if (L2 <= R2) {
                            count2 = wt->query(L2, R2, Y0);
                        }
                        total += count1 + count2;
                    } else {
                        int L = in_time[z];
                        int R = out_time[z];
                        if (L <= R) {
                            total += wt->query(L, R, Y0);
                        }
                    }
                }
                return total;
            };

            while (low_val < high_val) {
                ll mid_val = (low_val + high_val) / 2;
                if (count_in_row(mid_val) >= r) {
                    high_val = mid_val;
                } else {
                    low_val = mid_val + 1;
                }
            }

            ll ans = (x0 - 1) * n * 1LL * n + low_val;
            cout << ans << '\n';
        }
    }

    delete wt;

    return 0;
}