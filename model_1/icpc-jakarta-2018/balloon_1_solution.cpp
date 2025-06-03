#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int N;
    ll M;
    cin >> N >> M;
    vector<ll> A(N), B(N);
    for (int i = 0; i < N; i++) cin >> A[i];
    for (int i = 0; i < N; i++) cin >> B[i];

    int kA = 0;
    ll sumA = 0;
    for (int i = 0; i < N; i++) {
        if (sumA + A[i] <= M) {
            sumA += A[i];
            kA++;
        } else {
            break;
        }
    }

    if (kA == 0) {
        cout << -1 << endl;
        return 0;
    }

    vector<ll> prefixB;
    ll sumB = 0;
    for (int i = 0; i < kA; i++) {
        sumB += B[i];
        prefixB.push_back(sumB);
    }

    bool prefixB_exceeds_M = false;
    for (int i = 0; i < kA; i++) {
        if (prefixB[i] > M) {
            prefixB_exceeds_M = true;
            break;
        }
    }
    if (prefixB_exceeds_M) {
        cout << 0 << endl;
        return 0;
    }

    int best_cost = INT_MAX;
    vector<int> candidate_i;
    for (int i = 0; i < kA; i++) {
        ll a = M + 1 - prefixB[i];
        ll cost_i;
        if (a <= 0) {
            cost_i = 0;
        } else {
            cost_i = (a - 1) / B[i] + 1;
        }
        if (cost_i < best_cost) {
            best_cost = cost_i;
            candidate_i.clear();
            candidate_i.push_back(i);
        } else if (cost_i == best_cost) {
            candidate_i.push_back(i);
        }
    }

    if (best_cost > kA) {
        cout << -1 << endl;
        return 0;
    }

    if (best_cost == 0) {
        cout << 0 << endl;
        return 0;
    }

    vector<ll> balloons;
    ll cur = 0;
    for (int i = 0; i < best_cost; i++) {
        cur += A[i];
        balloons.push_back(cur);
    }

    ll M1 = 0;
    for (int j = 0; j < best_cost; j++) {
        ll candidate = balloons[j] + (best_cost - 1 - j);
        if (candidate > M1) M1 = candidate;
    }

    for (int idx = 0; idx < candidate_i.size(); idx++) {
        int i0 = candidate_i[idx];
        ll s_i0 = (i0 == 0) ? 0 : prefixB[i0 - 1];
        ll L = max(s_i0, M - B[i0] + 1);
        ll R = min(M, s_i0 + (ll)best_cost * B[i0] - 1);

        if (balloons.back() > R) {
            continue;
        }

        if (max(L + best_cost - 1, M1) <= R) {
            vector<ll> pops;
            ll current = L;
            bool valid = true;
            for (int j = 0; j < best_cost; j++) {
                ll p = max(current, balloons[j]);
                if (p > R) {
                    valid = false;
                    break;
                }
                pops.push_back(p);
                current = p + 1;
            }
            if (valid) {
                cout << best_cost << "\n";
                for (int j = 0; j < best_cost; j++) {
                    cout << pops[j];
                    if (j < best_cost - 1) cout << " ";
                }
                cout << "\n";
                return 0;
            }
        }
    }

    cout << -1 << endl;
    return 0;
}