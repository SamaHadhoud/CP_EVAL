#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    
    long long T = 0, S = 0;
    long long tot0 = 0, tot1 = 0;
    bool has0Major = false, has1Major = false, hasTie = false;
    long long minDiff = LLONG_MAX;
    
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        long long len = (long long)s.size();
        long long z = 0;
        for (char c : s) if (c == '0') ++z;
        long long o = len - z;
        
        T += len;
        tot0 += z;
        tot1 += o;
        S += max(z, o);
        
        if (z > o) has0Major = true;
        else if (o > z) has1Major = true;
        else hasTie = true;
        
        long long diff = (z > o) ? (z - o) : (o - z);
        if (diff < minDiff) minDiff = diff;
    }
    
    long long ans = 0;
    if (tot0 == 0 || tot1 == 0) {
        ans = 0;
    } else if (hasTie || (has0Major && has1Major)) {
        ans = T - S;
    } else {
        ans = T - S + minDiff;
    }
    
    cout << ans << '\n';
    return 0;
}