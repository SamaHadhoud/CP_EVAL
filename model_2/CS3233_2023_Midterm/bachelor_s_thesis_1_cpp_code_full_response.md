```cpp
#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val;
    int id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    auto count_inversions = [&](const vector<int>& v) -> long long {
        long long inv = 0;
        for (int i = 0; i < (int)v.size(); ++i)
            for (int j = i + 1; j < (int)v.size(); ++j)
                if (v[i] > v[j]) ++inv;
        return inv;
    };
    auto count_unequal_pairs = [&](const vector<int>& v) -> long long {
        long long U = 0;
        for (int i = 0; i < (int)v.size(); ++i)
            for (int j = i + 1; j < (int)v.size(); ++j)
                if (v[i] != v[j]) ++U;
        return U;
    };

    if (k == n) {
        long long inv = count_inversions(a);
        long long U = count_unequal_pairs(a);
        long long best = min(inv, U - inv);
        cout << best << "\n";
        if (inv <= U - inv) {
            cout << 0 << "\n";
        } else {
            cout << 1 << "\n";
            cout << string(n, '1') << "\n";
        }
        return 0;
    }

    // k < n: construct operations to sort (0 inversions)
    vector<Node> cur(n);
    for (int i = 0; i < n; ++i) cur[i] = {a[i], i};
    vector<string> ops;

    auto apply_op = [&](const vector<int>& pick) {
        // pick: 0-based indices selected, size must be exactly k
        string s(n, '0');
        vector<char> chosen(n, 0);
        for (int idx : pick) {
            s[idx] = '1';
            chosen[idx] = 1;
        }
        vector<Node> chosenSeq, restSeq;
        chosenSeq.reserve(k);
        restSeq.reserve(n - k);
        for (int i = 0; i < n; ++i) {
            if (chosen[i]) chosenSeq.push_back(cur[i]);
            else restSeq.push_back(cur[i]);
        }
        reverse(chosenSeq.begin(), chosenSeq.end());
        vector<Node> next;
        next.reserve(n);
        next.insert(next.end(), chosenSeq.begin(), chosenSeq.end());
        next.insert(next.end(), restSeq.begin(), restSeq.end());
        cur.swap(next);
        ops.push_back(s);
    };

    auto first_k = [&]() {
        vector<int> pick;
        for (int i = 0; i < k; ++i) pick.push_back(i);
        return pick;
    };
    auto first_k_minus1_and_pos = [&](int pos0) {
        vector<int> pick;
        for (int i = 0; i < k - 1; ++i) pick.push_back(i);
        pick.push_back(pos0);
        sort(pick.begin(), pick.end());
        pick.erase(unique(pick.begin(), pick.end()), pick.end());
        // Must be exactly k elements
        return pick;
    };
    auto first_k_except_with_kplus1 = [&](int exclude0) {
        vector<int> pick;
        for (int i = 0; i < k; ++i) {
            if (i == exclude0) continue;
            pick.push_back(i);
        }
        pick.push_back(k); // index of position k+1 (0-based)
        sort(pick.begin(), pick.end());
        return pick;
    };
    auto range_inclusive = [&](int L0, int R0) {
        vector<int> pick;
        for (int i = L0; i <= R0; ++i) pick.push_back(i);
        return pick;
    };
    auto find_rightmost_value = [&](int value) {
        for (int i = n - 1; i >= 0; --i) if (cur[i].val == value) return i;
        return -1;
    };
    auto find_by_id = [&](int id) {
        for (int i = 0; i < n; ++i) if (cur[i].id == id) return i;
        return -1;
    };

    auto move_to_front = [&](int pos0) {
        // Move element at 0-based pos0 to the very front, preserving order of others.
        if (pos0 == 0) return;
        if (k == 1) {
            // Single selection moves it to front
            apply_op(vector<int>{pos0});
            return;
        }
        int target_id = cur[pos0].id;

        if (pos0 <= k - 2) {
            // Eject: two ops
            apply_op(first_k_except_with_kplus1(pos0));
            apply_op(first_k());
            pos0 = find_by_id(target_id); // should be k (0-based), but recompute safely
        }
        // Now pos0 >= k-1 (1-based position >= k)
        apply_op(first_k_minus1_and_pos(pos0));
        apply_op(first_k());
        apply_op(first_k_minus1_and_pos(k));          // pick first k-1 and position k+1 (0-based k)
        apply_op(range_inclusive(1, k));              // pick positions 2..k+1 (0-based 1..k)
    };

    vector<int> T = a;
    sort(T.begin(), T.end()); // nondecreasing

    for (int i = n - 1; i >= 0; --i) {
        int val = T[i];
        int pos = find_rightmost_value(val);
        if (pos == -1) { /* should not happen */ }
        move_to_front(pos);
    }

    cout << 0 << "\n";
    cout << ops.size() << "\n";
    for (auto &s : ops) cout << s << "\n";
    return 0;
}
```