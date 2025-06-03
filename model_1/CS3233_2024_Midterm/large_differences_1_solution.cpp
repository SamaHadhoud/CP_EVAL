#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>
#include <climits>
using namespace std;

bool can(int X, const vector<int>& A) {
    int n = A.size();
    if (n == 0) return true;

    vector<int> candidates;
    candidates.push_back(A[0]);
    candidates.push_back(A[n-1]);
    int mid = n / 2;
    candidates.push_back(A[mid]);
    if (mid - 1 >= 0) 
        candidates.push_back(A[mid-1]);
    if (mid + 1 < n) 
        candidates.push_back(A[mid+1]);

    int best_len = 0;
    int best_start_idx = 0, best_end_idx = 0;
    int i = 0;
    while (i < n) {
        int j = i;
        while (j + 1 < n && (A[j+1] - A[j] < X)) {
            j++;
        }
        int current_len = j - i + 1;
        if (current_len > best_len) {
            best_len = current_len;
            best_start_idx = i;
            best_end_idx = j;
        }
        i = j + 1;
    }
    if (best_len > 0) {
        candidates.push_back(A[best_start_idx]);
        candidates.push_back(A[best_end_idx]);
    }

    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    for (int start : candidates) {
        multiset<int> ms;
        for (int num : A) {
            ms.insert(num);
        }
        auto it_start = ms.find(start);
        if (it_start == ms.end()) 
            continue;
        ms.erase(it_start);
        int last = start;
        bool valid = true;
        for (int step = 1; step < n; step++) {
            auto it_next = ms.lower_bound(last + X);
            if (it_next != ms.end()) {
                int val = *it_next;
                ms.erase(it_next);
                last = val;
            } else {
                it_next = ms.upper_bound(last - X);
                if (it_next == ms.begin()) {
                    valid = false;
                    break;
                }
                --it_next;
                int val = *it_next;
                if (last - val < X) {
                    valid = false;
                    break;
                }
                ms.erase(it_next);
                last = val;
            }
        }
        if (valid) {
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<int> H(n);
    for (int i = 0; i < n; i++) {
        cin >> H[i];
    }
    sort(H.begin(), H.end());

    long long low = 0;
    long long high = (n > 1) ? (H[n-1] - H[0]) : 0;
    long long ans = 0;

    while (low <= high) {
        long long mid = (low + high) / 2;
        if (can(static_cast<int>(mid), H)) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    cout << ans << endl;
    return 0;
}