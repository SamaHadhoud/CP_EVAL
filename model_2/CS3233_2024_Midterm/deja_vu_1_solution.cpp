#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long cost; // total penalty of the represented full combination
    int p;          // highest opened position (0..k-1)
    int x;          // absolute index used at position p
    bool operator<(const Node& other) const {
        // min-heap by cost
        if (cost != other.cost) return cost > other.cost;
        // tie-breakers to keep heap stable (not required for correctness)
        if (p != other.p) return p > other.p;
        return x > other.x;
    }
};

static inline void append_i64(string& out, long long v) {
    if (v == 0) { out.push_back('0'); out.push_back('\n'); return; }
    if (v < 0) { out.push_back('-'); v = -v; }
    char buf[24];
    int len = 0;
    while (v > 0) {
        buf[len++] = char('0' + (v % 10));
        v /= 10;
    }
    for (int i = len - 1; i >= 0; --i) out.push_back(buf[i]);
    out.push_back('\n');
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, k, l;
    if (!(cin >> n >> k >> l)) return 0;
    vector<long long> H(n);
    for (int i = 0; i < n; ++i) cin >> H[i];

    // Find smallest T such that C(T, k) >= l, or T = n if not reached
    long long T;
    {
        long long t = k;
        long long bin = 1; // C(k, k)
        while (t < n && bin < l) {
            __int128 num = (__int128)bin * (t + 1);
            long long den = (t + 1) - k;
            bin = (long long)(num / den); // exact division
            ++t;
        }
        T = (long long)t;
    }

    // Keep only top T elements in non-increasing order
    if (T < n) {
        nth_element(H.begin(), H.begin() + T, H.end(), greater<long long>());
        H.resize(T);
    }
    sort(H.begin(), H.end(), greater<long long>());
    // Now A is H[0..T-1] sorted non-increasingly
    const vector<long long>& A = H;

    // Base sum S0 = sum of top k elements
    long long S0 = 0;
    for (int i = 0; i < k; ++i) S0 += A[i];

    // Priority queue for best-first enumeration
    priority_queue<Node> pq;
    pq.push(Node{0, 0, 0});

    long long printed = 0;
    string out;
    out.reserve((size_t)min<long long>(l, 1000000) * 12);

    // Helper lambda: check if advancing same position is possible
    auto can_advance_same = [&](int p, int x) -> bool {
        // Need x + (k - p) < T  <=>  x < T - k + p
        return (long long)x < (long long)T - k + p;
    };
    // Helper lambda: check if opening next position is possible
    auto can_open_next = [&](int p, int x) -> bool {
        // Need x + 1 <= T - (k - (p+1))  <=>  x <= T - k + p
        return (p + 1 < k) && ((long long)x <= (long long)T - k + p);
    };

    while (printed < l && !pq.empty()) {
        Node cur = pq.top(); pq.pop();

        if (cur.p == k - 1) {
            // Full combination represented; print its sum
            append_i64(out, S0 - cur.cost);
            ++printed;
        }

        // Generate children
        if (can_advance_same(cur.p, cur.x)) {
            // Increasing x pushes positions p..k-1 by +1; the sum decreases by:
            // A[x] - A[x + (k - p)]
            long long inc = A[cur.x] - A[cur.x + (k - cur.p)];
            pq.push(Node{cur.cost + inc, cur.p, cur.x + 1});
        }
        if (can_open_next(cur.p, cur.x)) {
            // Open next position at minimal value; cost unchanged
            pq.push(Node{cur.cost, cur.p + 1, cur.x + 1});
        }
    }

    // If we couldn't produce l outputs, fill with -1
    while (printed < l) {
        append_i64(out, -1);
        ++printed;
    }

    cout << out;
    return 0;
}