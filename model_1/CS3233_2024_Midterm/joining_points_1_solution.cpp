#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <tuple>
#include <climits>
using namespace std;

typedef long long LL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<LL> xs, ys;
    vector<pair<LL,LL>> points;
    for (int i=0; i<n; i++) {
        LL x, y;
        cin >> x >> y;
        xs.push_back(x);
        ys.push_back(y);
        points.push_back({x,y});
    }

    LL min_x = *min_element(xs.begin(), xs.end());
    LL max_x = *max_element(xs.begin(), xs.end());
    LL min_y = *min_element(ys.begin(), ys.end());
    LL max_y = *max_element(ys.begin(), ys.end());

    LL s1 = max_x - min_x;
    LL s2 = max_y - min_y;
    LL s0 = max(s1, s2);

    set<tuple<LL,LL,LL,LL>> candSet;

    candSet.insert(make_tuple(min_x, min_x + s0, min_y, min_y + s0));
    candSet.insert(make_tuple(min_x, min_x + s0, max_y - s0, max_y));
    candSet.insert(make_tuple(max_x - s0, max_x, min_y, min_y + s0));
    candSet.insert(make_tuple(max_x - s0, max_x, max_y - s0, max_y));

    if (s1 >= s2) {
        candSet.insert(make_tuple(min_x, max_x, min_y, min_y + s1));
        candSet.insert(make_tuple(min_x, max_x, max_y - s1, max_y));
    }
    if (s2 >= s1) {
        candSet.insert(make_tuple(min_x, min_x + s2, min_y, max_y));
        candSet.insert(make_tuple(max_x - s2, max_x, min_y, max_y));
    }

    for (auto cand : candSet) {
        LL x1, x2, y1, y2;
        tie(x1, x2, y1, y2) = cand;

        bool valid = true;
        for (int i=0; i<n; i++) {
            LL x = points[i].first;
            LL y = points[i].second;
            if (x == x1 || x == x2 || y == y1 || y == y2) {
                continue;
            }
            valid = false;
            break;
        }
        if (valid) {
            cout << x1 << " " << x2 << " " << y1 << " " << y2 << endl;
            return 0;
        }
    }

    cout << "Impossible" << endl;
    return 0;
}