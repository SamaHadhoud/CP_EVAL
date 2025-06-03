#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    int N;
    cin >> N;
    vector<string> S(N);
    for (int i = 0; i < N; i++) {
        cin >> S[i];
    }

    vector<vector<string>> A_list(N);
    for (int i = 0; i < N; i++) {
        string s = S[i];
        set<string> seen;
        int n = s.length();
        for (int mask = 1; mask < (1 << n); mask++) {
            string t = "";
            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) {
                    t += s[j];
                }
            }
            seen.insert(t);
        }
        vector<string> A(seen.begin(), seen.end());
        sort(A.begin(), A.end());
        A_list[i] = A;
    }

    vector<int> dp_prev;
    for (string t : A_list[0]) {
        dp_prev.push_back(t.length());
    }

    for (int i = 1; i < N; i++) {
        vector<string> prev_list = A_list[i-1];
        vector<int> prefix_max(dp_prev.size(), 0);
        if (dp_prev.size() > 0) {
            prefix_max[0] = dp_prev[0];
            for (int j = 1; j < dp_prev.size(); j++) {
                prefix_max[j] = max(prefix_max[j-1], dp_prev[j]);
            }
        }

        vector<string> current_list = A_list[i];
        vector<int> dp_current(current_list.size(), -1000000000);

        for (int j = 0; j < current_list.size(); j++) {
            string t = current_list[j];
            auto it = lower_bound(prev_list.begin(), prev_list.end(), t);
            int pos = it - prev_list.begin();
            if (pos > 0) {
                dp_current[j] = prefix_max[pos-1] + t.length();
            }
        }
        dp_prev = dp_current;
    }

    int ans = -1;
    if (!dp_prev.empty()) {
        ans = *max_element(dp_prev.begin(), dp_prev.end());
        if (ans < 0) {
            ans = -1;
        }
    } else {
        ans = -1;
    }

    cout << ans << endl;

    return 0;
}