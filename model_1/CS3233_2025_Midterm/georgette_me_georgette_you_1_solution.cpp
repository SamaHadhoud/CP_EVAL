#include <iostream>
#include <vector>
using namespace std;

const int MAX_N = 1000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int T;
    long long M;
    cin >> T >> M;

    vector<long long> fact(MAX_N + 1);
    vector<long long> involution(MAX_N + 1);

    fact[0] = 1;
    for (int i = 1; i <= MAX_N; i++) {
        fact[i] = fact[i - 1] * i % M;
    }

    involution[0] = 1;
    if (MAX_N >= 1) {
        involution[1] = 1;
    }
    for (int i = 2; i <= MAX_N; i++) {
        long long temp = 1LL * (i - 1) * involution[i - 2] % M;
        involution[i] = (involution[i - 1] + temp) % M;
    }

    while (T--) {
        int N;
        cin >> N;
        long long ans = (fact[N] - involution[N] + M) % M;
        cout << ans << '\n';
    }

    return 0;
}