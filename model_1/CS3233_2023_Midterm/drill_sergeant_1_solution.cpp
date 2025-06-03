#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int m;
    cin >> n >> m;

    unordered_set<unsigned long long> dislikeSet;
    for (int i = 0; i < m; i++) {
        long long x, y;
        cin >> x >> y;
        if (x > y) swap(x, y);
        unsigned long long key = (static_cast<unsigned long long>(x) << 32) | static_cast<unsigned long long>(y);
        dislikeSet.insert(key);
    }

    auto has_dislike = [&](long long a, long long b) -> bool {
        if (a > b) swap(a, b);
        unsigned long long key = (static_cast<unsigned long long>(a) << 32) | static_cast<unsigned long long>(b);
        return dislikeSet.find(key) != dislikeSet.end();
    };

    set<long long> line;
    long long total = 0;

    auto get_discontentment = [&](long long cat) -> long long {
        auto it = line.find(cat);
        long long prev = -1, next = -1;
        if (it != line.begin()) {
            auto prev_it = it;
            prev_it--;
            prev = *prev_it;
        }
        auto next_it = it;
        next_it++;
        if (next_it != line.end()) {
            next = *next_it;
        }

        if (prev != -1 && next != -1) {
            bool d_prev = has_dislike(cat, prev);
            bool d_next = has_dislike(cat, next);
            if (d_prev && d_next) return 3233;
            else if (d_prev) return 323;
            else if (d_next) return 32;
            else return 3;
        } else if (prev != -1) {
            if (has_dislike(cat, prev)) return 323;
            else return 3;
        } else if (next != -1) {
            if (has_dislike(cat, next)) return 32;
            else return 3;
        } else {
            return 3;
        }
    };

    int q;
    cin >> q;
    vector<long long> results;

    for (int i = 0; i < q; i++) {
        int d;
        long long z;
        cin >> d >> z;

        if (d == 1) {
            auto it = line.lower_bound(z);
            long long prev = -1, next = -1;
            if (it != line.begin()) {
                auto prev_it = it;
                prev_it--;
                prev = *prev_it;
            }
            if (it != line.end()) {
                next = *it;
            }

            if (prev != -1) {
                total -= get_discontentment(prev);
            }
            if (next != -1) {
                total -= get_discontentment(next);
            }

            line.insert(z);
            if (prev != -1) {
                total += get_discontentment(prev);
            }
            if (next != -1) {
                total += get_discontentment(next);
            }
            total += get_discontentment(z);
        } else {
            auto it = line.find(z);
            long long prev = -1, next = -1;
            if (it != line.begin()) {
                auto prev_it = it;
                prev_it--;
                prev = *prev_it;
            }
            auto next_it = it;
            next_it++;
            if (next_it != line.end()) {
                next = *next_it;
            }

            total -= get_discontentment(z);
            if (prev != -1) {
                total -= get_discontentment(prev);
            }
            if (next != -1) {
                total -= get_discontentment(next);
            }

            line.erase(it);

            if (prev != -1) {
                total += get_discontentment(prev);
            }
            if (next != -1) {
                total += get_discontentment(next);
            }
        }
        results.push_back(total);
    }

    for (long long res : results) {
        cout << res << '\n';
    }

    return 0;
}