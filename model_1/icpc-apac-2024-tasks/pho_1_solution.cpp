#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;

    long long total_contestants = 0;
    long long saving = 0;
    long long total_zeros = 0, total_ones = 0;
    bool has0 = false;
    bool has1 = false;
    vector<int> force0;
    vector<int> force1;

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        total_contestants += s.size();
        int count0 = 0;
        for (char c : s) {
            if (c == '0') count0++;
        }
        int count1 = s.size() - count0;
        total_zeros += count0;
        total_ones += count1;

        saving += max(count0, count1);

        if (count0 >= count1) {
            has0 = true;
        }
        if (count1 >= count0) {
            has1 = true;
        }

        if (count0 < count1) {
            force0.push_back(count1 - count0);
        } else if (count1 < count0) {
            force1.push_back(count0 - count1);
        }
    }

    if (total_zeros > 0 && !has0) {
        int min_force0 = *min_element(force0.begin(), force0.end());
        saving -= min_force0;
    }
    if (total_ones > 0 && !has1) {
        int min_force1 = *min_element(force1.begin(), force1.end());
        saving -= min_force1;
    }

    long long moves = total_contestants - saving;
    cout << moves << '\n';

    return 0;
}