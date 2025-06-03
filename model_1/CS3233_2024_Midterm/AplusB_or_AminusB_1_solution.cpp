#include <iostream>
#include <cstring>
using namespace std;

typedef long long ll;

ll L_val, R_val;
ll dp[33][2][2][2][2][2];

ll dfs(int i, int carry, bool tl, bool th, bool ta, int prev) {
    if (i == 32) {
        return (carry == 0) ? 1 : 0;
    }
    if (dp[i][carry][tl][th][ta][prev] != -1) 
        return dp[i][carry][tl][th][ta][prev];
    
    ll res = 0;
    int L_i = (L_val >> i) & 1;
    int R_i = (R_val >> i) & 1;

    int low_B = tl ? L_i : 0;
    int high_B = th ? R_i : 1;

    for (int b = low_B; b <= high_B; b++) {
        for (int x = 0; x < 2; x++) {
            if (i > 0 && x == 1 && prev == 1) 
                continue;

            int total = b + x + carry;
            int bit_A = total & 1;
            int new_carry = total >> 1;

            if (ta) {
                if (bit_A > R_i) 
                    continue;
            }

            bool new_tl = tl && (b == L_i);
            bool new_th = th && (b == R_i);
            bool new_ta = ta && (bit_A == R_i);

            res += dfs(i+1, new_carry, new_tl, new_th, new_ta, b);
        }
    }

    return dp[i][carry][tl][th][ta][prev] = res;
}

int main() {
    cin >> L_val >> R_val;
    memset(dp, -1, sizeof(dp));
    ll ans = dfs(0, 0, 1, 1, 1, 0);
    cout << ans << endl;
    return 0;
}