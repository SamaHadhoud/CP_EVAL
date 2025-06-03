#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const long long mod = 1000003233;
const int MAXN = 300000;
int a[MAXN];

long long compute_cross(int l, int r, int mid) {
    int lenL = mid - l + 1;
    int lenR = r - mid;
    if (lenR == 0) 
        return 0;

    vector<long long> Lmin_arr(lenL);
    vector<long long> Lmax_arr(lenL);
    Lmin_arr[lenL-1] = a[mid];
    Lmax_arr[lenL-1] = a[mid];
    for (int i = lenL-2; i >= 0; i--) {
        int idx = l + i;
        Lmin_arr[i] = min(static_cast<long long>(a[idx]), Lmin_arr[i+1]);
        Lmax_arr[i] = max(static_cast<long long>(a[idx]), Lmax_arr[i+1]);
    }

    vector<long long> Rmin_arr(lenR);
    vector<long long> Rmax_arr(lenR);
    vector<long long> Rprod_arr(lenR);
    vector<long long> PrefRmin(lenR+1, 0);
    vector<long long> PrefRmax(lenR+1, 0);
    vector<long long> PrefRprod(lenR+1, 0);

    Rmin_arr[0] = a[mid+1];
    Rmax_arr[0] = a[mid+1];
    Rprod_arr[0] = (Rmin_arr[0] % mod) * (Rmax_arr[0] % mod) % mod;
    PrefRmin[1] = Rmin_arr[0];
    PrefRmax[1] = Rmax_arr[0];
    PrefRprod[1] = Rprod_arr[0];

    for (int j = 1; j < lenR; j++) {
        int global_j = mid + 1 + j;
        Rmin_arr[j] = min(Rmin_arr[j-1], static_cast<long long>(a[global_j]));
        Rmax_arr[j] = max(Rmax_arr[j-1], static_cast<long long>(a[global_j]));
        Rprod_arr[j] = (Rmin_arr[j] % mod) * (Rmax_arr[j] % mod) % mod;
        PrefRmin[j+1] = PrefRmin[j] + Rmin_arr[j];
        PrefRmax[j+1] = PrefRmax[j] + Rmax_arr[j];
        PrefRprod[j+1] = (PrefRprod[j] + Rprod_arr[j]) % mod;
    }

    long long cross_ans = 0;
    for (int i = 0; i < lenL; i++) {
        long long curLmin = Lmin_arr[i];
        long long curLmax = Lmax_arr[i];

        int low1 = 0, high1 = lenR - 1;
        int j1 = -1;
        while (low1 <= high1) {
            int mid1 = (low1 + high1) / 2;
            if (Rmin_arr[mid1] >= curLmin) {
                j1 = mid1;
                low1 = mid1 + 1;
            } else {
                high1 = mid1 - 1;
            }
        }

        int low2 = 0, high2 = lenR - 1;
        int j2 = -1;
        while (low2 <= high2) {
            int mid2 = (low2 + high2) / 2;
            if (Rmax_arr[mid2] <= curLmax) {
                j2 = mid2;
                low2 = mid2 + 1;
            } else {
                high2 = mid2 - 1;
            }
        }

        int j_low, j_high;
        if (j1 == -1 && j2 == -1) {
            j_low = -1;
            j_high = -1;
        } else if (j1 == -1) {
            j_low = -1;
            j_high = j2;
        } else if (j2 == -1) {
            j_low = -1;
            j_high = j1;
        } else {
            j_low = min(j1, j2);
            j_high = max(j1, j2);
        }

        long long S1 = 0, S2 = 0, S3 = 0;

        if (j_low >= 0) {
            long long count1 = j_low + 1;
            long long temp = (curLmin % mod) * (curLmax % mod) % mod;
            S1 = (count1 % mod) * temp % mod;
        }

        if (j_high >= j_low + 1) {
            if (j1 <= j2) {
                long long sumRmin = PrefRmin[j_high+1] - PrefRmin[j_low+1];
                S2 = (sumRmin % mod) * (curLmax % mod) % mod;
            } else {
                long long sumRmax = PrefRmax[j_high+1] - PrefRmax[j_low+1];
                S2 = (curLmin % mod) * (sumRmax % mod) % mod;
            }
        }

        if (j_high + 1 <= lenR - 1) {
            S3 = (PrefRprod[lenR] - PrefRprod[j_high+1] + mod) % mod;
        }

        long long total_i = (S1 + S2 + S3) % mod;
        cross_ans = (cross_ans + total_i) % mod;
    }
    cross_ans %= mod;
    if (cross_ans < 0) cross_ans += mod;
    return cross_ans;
}

long long solve(int l, int r) {
    if (l == r) {
        return (static_cast<long long>(a[l]) * a[l]) % mod;
    }
    int mid = (l + r) / 2;
    long long left_ans = solve(l, mid);
    long long right_ans = solve(mid+1, r);
    long long cross_ans = compute_cross(l, r, mid);
    long long ans = (left_ans + right_ans + cross_ans) % mod;
    if (ans < 0) ans += mod;
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    long long ans = solve(0, n-1);
    cout << ans << endl;
    return 0;
}