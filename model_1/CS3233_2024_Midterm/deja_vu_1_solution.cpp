#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <tuple>
using namespace std;
using ll = long long;

struct State {
    ll sum;
    int i, j;
    int depth;
    bool operator<(const State& other) const {
        return sum < other.sum;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, l;
    cin >> n >> k >> l;
    vector<ll> A(n);
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    sort(A.rbegin(), A.rend());

    ll base = 0;
    for (int i = 0; i < k; i++) {
        base += A[i];
    }

    vector<ll> ans;
    ans.push_back(base);

    if (l > 1) {
        priority_queue<State> heap;
        set<tuple<int, int, int>> visited;

        if (k < n) {
            for (int i = k - 1; i >= 0; i--) {
                ll new_sum = base - A[i] + A[k];
                State st = {new_sum, i, k, 1};
                auto key = make_tuple(i, k, 1);
                if (visited.find(key) == visited.end()) {
                    visited.insert(key);
                    heap.push(st);
                }
            }
        }

        int count = 1;
        while (count < l && !heap.empty()) {
            State cur = heap.top();
            heap.pop();
            ans.push_back(cur.sum);
            count++;

            int j_next = cur.j + 1;
            if (j_next < n) {
                ll sum1 = cur.sum - A[cur.j] + A[j_next];
                State st1 = {sum1, cur.i, j_next, cur.depth};
                auto key1 = make_tuple(cur.i, j_next, cur.depth);
                if (visited.find(key1) == visited.end()) {
                    visited.insert(key1);
                    heap.push(st1);
                }
            }

            int i_prev = cur.i - 1;
            if (i_prev >= 0 && j_next < n) {
                ll sum2 = cur.sum - A[i_prev] + A[j_next];
                State st2 = {sum2, i_prev, j_next, cur.depth + 1};
                auto key2 = make_tuple(i_prev, j_next, cur.depth + 1);
                if (visited.find(key2) == visited.end()) {
                    visited.insert(key2);
                    heap.push(st2);
                }
            }
        }

        while (count < l) {
            ans.push_back(-1);
            count++;
        }
    }

    for (ll a : ans) {
        cout << a << '\n';
    }
    return 0;
}