#include <iostream>
using namespace std;

const long long MOD = 998244353;

long long mod_exp(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    long long r, c;
    cin >> r >> c;
    long long exponent = (long long)r * c;
    cout << mod_exp(2, exponent, MOD) << endl;
    return 0;
}