#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <tuple>
using namespace std;

typedef unsigned long long ull;
#ifdef __SIZEOF_INT128__
typedef __int128 lll;
#else
#error "__int128 is not supported"
#endif

string to_string(lll num) {
    if (num == 0) return "0";
    string s;
    while (num) {
        s = char('0' + (num % 10)) + s;
        num /= 10;
    }
    return s;
}

bool is_sorted(const string& s) {
    for (int i = 0; i < (int)s.size() - 1; i++) {
        if (s[i] > s[i+1])
            return false;
    }
    return true;
}

int main() {
    vector<ull> valid_n;
    const ull maxX = 1000000000000000000ULL;

    queue<tuple<ull, int, int>> q;
    for (int d = 1; d <= 9; d++) {
        q.push(make_tuple(d, d, 1));
    }

    while (!q.empty()) {
        auto front = q.front();
        ull cur = get<0>(front);
        int last = get<1>(front);
        int len = get<2>(front);
        q.pop();
        lll nsq = (lll)cur * cur;
        string sq_str = to_string(nsq);
        if (is_sorted(sq_str)) {
            valid_n.push_back(cur);
        }

        if (len < 18) {
            for (int d = last; d <= 9; d++) {
                ull next_num = cur * 10ULL + (ull)d;
                if (next_num > maxX) 
                    break;
                q.push(make_tuple(next_num, d, len+1));
            }
        }
    }

    sort(valid_n.begin(), valid_n.end());

    int T;
    cin >> T;
    while (T--) {
        ull X;
        cin >> X;
        auto it = upper_bound(valid_n.begin(), valid_n.end(), X);
        cout << (it - valid_n.begin()) << '\n';
    }
    return 0;
}