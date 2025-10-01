```cpp
#include <bits/stdc++.h>
using namespace std;

struct Update {
    int pos, oldv, newv;
};

struct Query {
    int l, r, t, id;
    int lb, rb; // blocks for sorting
};

int N, M, K;
vector<int> P, arr, cnt;
int blockSize;

inline int ok(int c, int K) {
    return (c >= 1 && c <= K) ? 1 : 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M >> K;
    P.assign(N + 1, 0);
    for (int i = 1; i <= N; ++i) cin >> P[i];

    vector<Update> updates;
    updates.reserve(M);
    vector<Query> queries;
    queries.reserve(M);

    vector<int> cur = P;
    int t = 0; // number of updates seen so far
    for (int i = 0; i < M; ++i) {
        int q, a, b;
        cin >> q >> a >> b;
        if (q == 1) {
            updates.push_back({a, cur[a], b});
            cur[a] = b;
            ++t;
        } else {
            queries.push_back({a, b, t, (int)queries.size(), 0, 0});
        }
    }

    // Prepare for Mo's algorithm with time dimension
    arr = P;
    cnt.assign(N + 1, 0);

    // Block size for 3D Mo's ordering
    blockSize = max(1, (int)pow((double)N, 2.0 / 3.0));
    for (auto &q : queries) {
        q.lb = q.l / blockSize;
        q.rb = q.r / blockSize;
    }

    sort(queries.begin(), queries.end(), [](const Query &a, const Query &b) {
        if (a.lb != b.lb) return a.lb < b.lb;
        if (a.rb != b.rb) return (a.lb & 1) ? (a.rb > b.rb) : (a.rb < b.rb);
        return (a.rb & 1) ? (a.t > b.t) : (a.t < b.t);
    });

    auto add = [&](int pos, int &ans) {
        int x = arr[pos];
        int c = cnt[x];
        cnt[x] = c + 1;
        ans += ok(c + 1, K) - ok(c, K);
    };
    auto remove = [&](int pos, int &ans) {
        int x = arr[pos];
        int c = cnt[x];
        cnt[x] = c - 1;
        ans += ok(c - 1, K) - ok(c, K);
    };
    auto applyUpdate = [&](const Update &u, int L, int R, int &ans) {
        int pos = u.pos;
        if (L <= pos && pos <= R) remove(pos, ans);
        arr[pos] = u.newv;
        if (L <= pos && pos <= R) add(pos, ans);
    };
    auto undoUpdate = [&](const Update &u, int L, int R, int &ans) {
        int pos = u.pos;
        if (L <= pos && pos <= R) remove(pos, ans);
        arr[pos] = u.oldv;
        if (L <= pos && pos <= R) add(pos, ans);
    };

    vector<int> answer(queries.size(), 0);
    int L = 1, R = 0, Tcur = 0;
    int ans = 0;

    for (const auto &q : queries) {
        while (Tcur < q.t) {
            applyUpdate(updates[Tcur], L, R, ans);
            ++Tcur;
        }
        while (Tcur > q.t) {
            --Tcur;
            undoUpdate(updates[Tcur], L, R, ans);
        }
        while (R < q.r) { ++R; add(R, ans); }
        while (L > q.l) { --L; add(L, ans); }
        while (R > q.r) { remove(R, ans); --R; }
        while (L < q.l) { remove(L, ans); ++L; }

        answer[q.id] = ans;
    }

    for (int i = 0; i < (int)answer.size(); ++i) {
        cout << answer[i] << '\n';
    }
    return 0;
}
```