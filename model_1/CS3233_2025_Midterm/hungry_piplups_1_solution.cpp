#include <iostream>
#include <vector>
using namespace std;

const int MAX_U = 1001000;

int next_ptr[MAX_U + 2];
int timestamp[MAX_U + 2];
int now = 0;

int find(int x, int U) {
    if (x > U) {
        return x;
    }
    vector<int> stack;
    int cur = x;
    while (cur <= U) {
        if (timestamp[cur] != now) {
            break;
        }
        if (next_ptr[cur] == cur) {
            break;
        }
        stack.push_back(cur);
        cur = next_ptr[cur];
    }
    if (cur <= U && timestamp[cur] != now) {
        timestamp[cur] = now;
        next_ptr[cur] = cur;
    }
    for (int node : stack) {
        next_ptr[node] = cur;
    }
    return cur;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    for (int i = 0; i < N; i++) {
        cin >> A[i];
    }
    for (int i = 0; i < M; i++) {
        int type;
        cin >> type;
        if (type == 0) {
            int P, X;
            cin >> P >> X;
            A[P-1] = X;
        } else {
            int L, R, X_i;
            cin >> L >> R >> X_i;
            int len = R - L + 1;
            int U_bound = len + 1000;
            now++;
            for (int j = L-1; j <= R-1; j++) {
                int x = A[j];
                int y = find(x, U_bound);
                if (y <= U_bound) {
                    next_ptr[y] = find(y+1, U_bound);
                }
            }
            int current = 0;
            for (int cnt = 0; cnt < X_i; cnt++) {
                current = find(current+1, U_bound);
            }
            cout << current << '\n';
        }
    }
    return 0;
}