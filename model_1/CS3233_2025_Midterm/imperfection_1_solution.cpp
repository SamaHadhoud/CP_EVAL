#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const int MAX_V = 300000;
const int MAX_C = 300000;

class Fenw {
public:
    int n;
    vector<long long> tree;

    Fenw(int size) : n(size) {
        tree.assign(n+1, 0);
    }

    void update(int index, long long delta) {
        while (index <= n) {
            tree[index] += delta;
            index += index & -index;
        }
    }

    long long query(int index) {
        if (index <= 0) return 0;
        if (index > n) index = n;
        long long s = 0;
        while (index) {
            s += tree[index];
            index -= index & -index;
        }
        return s;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> A(n), B(n);
    for (int i = 0; i < n; i++) cin >> A[i];
    for (int i = 0; i < n; i++) cin >> B[i];

    vector<int> F(MAX_V+2, 0);
    vector<long long> sum_cost(MAX_V+2, 0);
    vector<vector<int>> costs_by_value(MAX_V+2);

    for (int i = 0; i < n; i++) {
        if (A[i] > MAX_V) continue;
        F[A[i]]++;
        sum_cost[A[i]] += B[i];
        costs_by_value[A[i]].push_back(B[i]);
    }

    vector<long long> suffix_sum(MAX_V+3, 0);
    for (int i = MAX_V; i >= 1; i--) {
        suffix_sum[i] = suffix_sum[i+1] + sum_cost[i];
    }

    vector<int> total_count_below(MAX_V+2, 0);
    for (int x = 1; x <= MAX_V; x++) {
        total_count_below[x] = total_count_below[x-1] + F[x-1];
    }

    vector<vector<long long>> prefix_x_arr(MAX_V+2);
    for (int v = 1; v <= MAX_V; v++) {
        if (F[v] > 0) {
            sort(costs_by_value[v].begin(), costs_by_value[v].end());
            vector<long long> arr(F[v]+1);
            arr[0] = 0;
            for (int i = 1; i <= F[v]; i++) {
                arr[i] = arr[i-1] + costs_by_value[v][i-1];
            }
            prefix_x_arr[v] = arr;
        }
    }

    Fenw cnt_tree(MAX_C);
    Fenw sum_tree(MAX_C);

    long long candidate_ans = LLONG_MAX;

    for (int x = 1; x <= MAX_V; x++) {
        if (x >= 2) {
            for (int c : costs_by_value[x-1]) {
                cnt_tree.update(c, 1);
                sum_tree.update(c, c);
            }
        }

        if (F[x] == 0) continue;

        long long fixed_cost = suffix_sum[x+1];
        int k0 = F[x];
        int t0 = total_count_below[x];
        int D0 = max(0, t0 - k0 + 1);

        if (D0 == 0) {
            if (fixed_cost < candidate_ans) {
                candidate_ans = fixed_cost;
            }
        } else {
            long long best_step2 = LLONG_MAX;
            for (int b = 0; b < k0; b++) {
                long long a = (long long)D0 + b;

                int low = 1, high = MAX_C;
                int T = MAX_C;
                while (low <= high) {
                    int mid = (low + high) >> 1;
                    long long cnt_here = cnt_tree.query(mid);
                    if (cnt_here >= a) {
                        T = mid;
                        high = mid - 1;
                    } else {
                        low = mid + 1;
                    }
                }

                long long cnt1 = cnt_tree.query(T-1);
                long long sum1 = sum_tree.query(T-1);
                long long s_nonx = sum1 + (a - cnt1) * T;

                long long s_x = prefix_x_arr[x][b];
                long long total_var = s_nonx + s_x;
                if (total_var < best_step2) {
                    best_step2 = total_var;
                }
            }
            long long total_cost = fixed_cost + best_step2;
            if (total_cost < candidate_ans) {
                candidate_ans = total_cost;
            }
        }
    }

    cout << candidate_ans << endl;

    return 0;
}