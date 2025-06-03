#include <iostream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <string>

using namespace std;

long long mod_pow(long long base, long long exp, long long mod) {
    base %= mod;
    long long res = 1;
    while (exp > 0) {
        if (exp & 1) {
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

int main() {
    long long n, m, k;
    cin >> n >> m >> k;
    vector<long long> digits;
    for (int i = 0; i < m; i++) {
        long long x;
        cin >> x;
        digits.push_back(x);
    }

    if (n == 1) {
        for (int i = 0; i < k; i++) {
            cout << digits[i];
        }
        cout << endl;
        return 0;
    }

    long long base1 = 131, base2 = 137;
    long long mod1 = 1000000007, mod2 = 1000000009;
    long long MOD = 2000000000;

    long long exponent = n - 2;
    long long base_power1 = 1;
    long long base_power2 = 1;

    if (exponent > 0) {
        base_power1 = mod_pow(base1, exponent, mod1);
        base_power2 = mod_pow(base2, exponent, mod2);
    }

    deque<long long> current_state;
    for (int i = 0; i < n - 1; i++) {
        current_state.push_back(digits[0]);
    }

    long long H1 = 0, H2 = 0;
    for (auto d : current_state) {
        H1 = (H1 * base1 + d) % mod1;
        if (H1 < 0) H1 += mod1;
        H2 = (H2 * base2 + d) % mod2;
        if (H2 < 0) H2 += mod2;
    }

    vector<char> s_list;
    for (auto d : current_state) {
        s_list.push_back('0' + d);
    }

    unordered_set<long long> visited;
    long long count = 0;
    long long total_len = n - 1;

    while (count < k && total_len < 100000) {
        bool found = false;
        for (int i = 0; i < m; i++) {
            long long d = digits[i];
            long long pin_h1 = (H1 * base1 + d) % mod1;
            if (pin_h1 < 0) pin_h1 += mod1;
            long long pin_h2 = (H2 * base2 + d) % mod2;
            if (pin_h2 < 0) pin_h2 += mod2;

            long long key = pin_h1 * MOD + pin_h2;
            if (visited.find(key) == visited.end()) {
                visited.insert(key);
                count++;
                s_list.push_back('0' + d);
                total_len++;
                long long front_digit = current_state.front();
                current_state.pop_front();
                current_state.push_back(d);

                H1 = (H1 - front_digit * base_power1) % mod1;
                H1 = (H1 * base1 + d) % mod1;
                if (H1 < 0) H1 += mod1;

                H2 = (H2 - front_digit * base_power2) % mod2;
                H2 = (H2 * base2 + d) % mod2;
                if (H2 < 0) H2 += mod2;

                found = true;
                break;
            }
        }

        if (!found) {
            long long d = digits[m - 1];
            s_list.push_back('0' + d);
            total_len++;
            long long front_digit = current_state.front();
            current_state.pop_front();
            current_state.push_back(d);

            H1 = (H1 - front_digit * base_power1) % mod1;
            H1 = (H1 * base1 + d) % mod1;
            if (H1 < 0) H1 += mod1;

            H2 = (H2 - front_digit * base_power2) % mod2;
            H2 = (H2 * base2 + d) % mod2;
            if (H2 < 0) H2 += mod2;
        }
    }

    string res = "";
    for (char c : s_list) {
        res += c;
    }
    cout << res << endl;
    return 0;
}