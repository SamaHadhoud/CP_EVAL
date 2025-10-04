#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<long long> a(N);
    for (int i = 0; i < N; ++i) cin >> a[i];

    sort(a.begin(), a.end());
    int t = N / 2;
    long long ans = LLONG_MAX;
    for (int i = 0; i < t; ++i) {
        ans = min(ans, a[i + t] - a[i]);
    }
    cout << ans << '\n';
    return 0;
}