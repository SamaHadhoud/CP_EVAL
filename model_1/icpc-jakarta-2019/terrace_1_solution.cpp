#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

int main() {
    int n, K, G1, G2, G3;
    cin >> n >> K >> G1 >> G2 >> G3;
    string s;
    cin >> s;

    int total_soil = 0;
    for (char c : s) {
        if (c == '.') total_soil++;
    }

    vector<int> rocks;
    for (int i = 0; i < n; i++) {
        if (s[i] == '#') rocks.push_back(i);
    }
    int m = rocks.size();

    if (m == 0) {
        long long best = 0;
        for (int x = 0; x <= min(n, K); x++) {
            long long value = 1LL * x * G1 + ((n - x) / 2) * G2;
            if (value > best) best = value;
        }
        cout << best << endl;
        return 0;
    }

    vector<int> segs;
    segs.push_back(rocks[0]);
    for (int i = 1; i < m; i++) {
        segs.push_back(rocks[i] - rocks[i-1] - 1);
    }
    segs.push_back(n - rocks.back() - 1);

    vector<bool> eligible(m, false);
    for (int i = 0; i < m; i++) {
        int pos = rocks[i];
        if (pos > 0 && pos < n-1 && s[pos-1] == '.' && s[pos+1] == '.') {
            eligible[i] = true;
        }
    }

    vector<int> arr;
    for (int i = 0; i < m; i++) {
        if (eligible[i]) {
            arr.push_back(rocks[i]);
        }
    }
    int t = arr.size();

    vector<int> dp_ind_set(t, 0);
    int max_T = 0;
    if (t > 0) {
        dp_ind_set[0] = 1;
        max_T = 1;
        for (int i = 1; i < t; i++) {
            dp_ind_set[i] = 1;
            for (int j = i-1; j >= 0; j--) {
                if (arr[i] - arr[j] >= 3) {
                    dp_ind_set[i] = max(dp_ind_set[i], 1 + dp_ind_set[j]);
                    break;
                }
            }
            if (dp_ind_set[i] < dp_ind_set[i-1]) {
                dp_ind_set[i] = dp_ind_set[i-1];
            }
            if (dp_ind_set[i] > max_T) max_T = dp_ind_set[i];
        }
    }

    long long best_ans = 0;
    for (int T = 0; T <= max_T; T++) {
        vector<int> current_lengths = segs;
        if (T > 0) {
            for (int i = 0; i < m; i++) {
                int reduction = 0;
                if (i == 0) {
                    if (i < m && eligible[i]) reduction = 1;
                } else if (i == m) {
                    if (i-1 < m && eligible[i-1]) reduction = 1;
                } else {
                    if (i-1 < m && eligible[i-1]) reduction++;
                    if (i < m && eligible[i]) reduction++;
                }
                current_lengths[i] = max(0, current_lengths[i] - reduction);
            }
        }

        long long total_value_soil = 0;
        int total_soil_remaining = 0;
        for (int L : current_lengths) {
            total_soil_remaining += L;
        }
        if (total_soil_remaining != total_soil - 2*T) {
            continue;
        }

        priority_queue<pair<long long, pair<int, int>>> heap;
        vector<int> current_x(current_lengths.size(), 0);
        vector<long long> seg_base_value(current_lengths.size(), 0);
        for (int i = 0; i < current_lengths.size(); i++) {
            int L = current_lengths[i];
            if (L == 0) {
                seg_base_value[i] = 0;
                continue;
            }
            seg_base_value[i] = (L / 2) * (long long)G2;
            long long gain;
            if (L % 2 == 1) {
                gain = G1;
            } else {
                gain = G1 - G2;
            }
            heap.push({gain, {i, 0}});
        }

        long long base_value = 0;
        for (long long val : seg_base_value) {
            base_value += val;
        }
        long long total_value = base_value;
        vector<int> x_used(current_lengths.size(), 0);
        int total_x_used = 0;
        vector<bool> still_active(current_lengths.size(), true);

        for (int token = 0; token < K; token++) {
            if (heap.empty()) break;
            auto top = heap.top();
            heap.pop();
            long long gain = top.first;
            int i = top.second.first;
            int current_x_i = top.second.second;

            if (current_x_i != x_used[i]) {
                continue;
            }

            if (gain <= 0) {
                break;
            }

            total_value += gain;
            x_used[i]++;
            total_x_used++;
            if (x_used[i] >= current_lengths[i]) {
                still_active[i] = false;
            } else {
                int L = current_lengths[i];
                int x = x_used[i];
                if ((L - x) % 2 == 1) {
                    gain = G1;
                } else {
                    gain = G1 - G2;
                }
                heap.push({gain, {i, x_used[i]}});
            }
        }

        long long candidate = total_value + 1LL * T * G3;
        if (candidate > best_ans) {
            best_ans = candidate;
        }
    }

    cout << best_ans << endl;
    return 0;
}