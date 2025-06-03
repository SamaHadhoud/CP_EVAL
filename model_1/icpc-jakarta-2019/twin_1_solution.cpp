#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    vector<pair<long long, long long>> lands;
    long long candidate_one = 0;
    for (int i = 0; i < n; i++) {
        long long L, W;
        cin >> L >> W;
        long long long_i = max(L, W);
        long long short_i = min(L, W);
        lands.push_back({long_i, short_i});
        candidate_one = max(candidate_one, long_i * short_i);
    }

    sort(lands.begin(), lands.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    long long candidate_two = 0;
    long long max_short = 0;
    for (int i = 0; i < n; i++) {
        long long long_i = lands[i].first;
        long long short_i = lands[i].second;
        if (max_short != 0) {
            long long temp = min(short_i, max_short);
            candidate_two = max(candidate_two, long_i * temp);
        }
        if (short_i > max_short) {
            max_short = short_i;
        }
    }

    if (candidate_one >= 2 * candidate_two) {
        long long half = candidate_one / 2;
        if (candidate_one % 2 == 0) {
            cout << half << ".0\n";
        } else {
            cout << half << ".5\n";
        }
    } else {
        cout << candidate_two << ".0\n";
    }
    return 0;
}