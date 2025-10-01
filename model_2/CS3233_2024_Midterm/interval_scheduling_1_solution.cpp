#include <bits/stdc++.h>
using namespace std;

int toMinutes(const string& s) {
    size_t p = s.find(':');
    int h = stoi(s.substr(0, p));
    int m = stoi(s.substr(p + 1));
    return h * 60 + m;
}

string toHHMM(int t) {
    int h = t / 60, m = t % 60;
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", h, m);
    return string(buf);
}

bool feasible(int L, const vector<int>& l, const vector<int>& r) {
    int n = (int)l.size();
    int a = l[0]; // a1 fixed
    for (int i = 0; i < n - 1; ++i) {
        int b = max(r[i], a + L); // earliest possible end
        if (b > l[i + 1]) return false; // would violate non-overlap or inclusion of next core
        a = b; // next start as early as possible
    }
    return a + L <= r[n - 1]; // last must fit with fixed end
}

void buildSchedule(int L, const vector<int>& l, const vector<int>& r,
                   vector<int>& a, vector<int>& b) {
    int n = (int)l.size();
    a.assign(n, 0);
    b.assign(n, 0);
    a[0] = l[0];
    for (int i = 0; i < n - 1; ++i) {
        b[i] = max(r[i], a[i] + L);
        a[i + 1] = b[i];
    }
    b[n - 1] = r[n - 1]; // last end fixed
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<int> l(N), r(N);
    for (int i = 0; i < N; ++i) {
        string s1, dash, s2;
        cin >> s1 >> dash >> s2; // s1="HH:MM", dash="-", s2="HH:MM"
        l[i] = toMinutes(s1);
        r[i] = toMinutes(s2);
    }
    
    int low = 0, high = r.back() - l.front();
    while (low < high) {
        int mid = (low + high + 1) / 2;
        if (feasible(mid, l, r)) low = mid;
        else high = mid - 1;
    }
    
    vector<int> a, bnd;
    buildSchedule(low, l, r, a, bnd);
    
    for (int i = 0; i < N; ++i) {
        cout << toHHMM(a[i]) << " - " << toHHMM(bnd[i]) << "\n";
    }
    return 0;
}