#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

const int mod = 998244353;
const int MAX = 300000;

int main() {
    vector<int> f(MAX + 1);
    f[1] = 1;
    if (MAX >= 2) {
        f[2] = 1;
    }
    for (int i = 3; i <= MAX; i++) {
        long long temp = (long long)f[i - 1] + f[i - 2];
        if (i % 3 == 0) {
            temp += 1;
        }
        f[i] = temp % mod;
    }

    int t;
    scanf("%d", &t);
    while (t--) {
        int n;
        scanf("%d", &n);
        printf("%d\n", f[n]);
    }
    return 0;
}