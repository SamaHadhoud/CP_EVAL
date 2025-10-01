#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int MAXA = 500; // given Ai, Xi <= 500
    int N, M;
    if (!(cin >> N >> M)) return 0;

    vector<int> A(N + 1);
    vector<ordered_set<int>> pos(MAXA + 1); // indices 0..500; pos[0] used only if input has 0 (just in case)

    for (int i = 1; i <= N; ++i) {
        int v; cin >> v;
        // As per limits Ai is within [1..500]; if 0 occurs, it is handled by pos[0] and merged into c[1] during queries.
        A[i] = v;
        pos[v].insert(i);
    }

    while (M--) {
        int type; 
        cin >> type;
        if (type == 0) {
            int P, X; 
            cin >> P >> X;
            if (A[P] == X) continue;
            pos[A[P]].erase(P);
            pos[X].insert(P);
            A[P] = X;
        } else {
            int L, R, Xth;
            cin >> L >> R >> Xth;

            long long backlog = 0;
            int holes = 0;
            int answer = -1;

            for (int p = 1; p <= MAXA; ++p) {
                int cnt;
                if (p == 1) {
                    // Merge starts at 0 into 1 to avoid iceberg 0 (if any)
                    cnt = int(pos[0].order_of_key(R + 1) - pos[0].order_of_key(L))
                        + int(pos[1].order_of_key(R + 1) - pos[1].order_of_key(L));
                } else {
                    cnt = int(pos[p].order_of_key(R + 1) - pos[p].order_of_key(L));
                }
                backlog += cnt;
                if (backlog > 0) {
                    backlog -= 1; // occupy p
                } else {
                    holes += 1;
                    if (holes == Xth) {
                        answer = p;
                        break;
                    }
                }
            }

            if (answer == -1) {
                long long tail = backlog;               // contiguous occupied tail after MAXA
                long long rem = Xth - holes;            // how many missing still needed
                long long res = (long long)MAXA + tail + rem;
                answer = (int)res;
            }

            cout << answer << '\n';
        }
    }
    return 0;
}