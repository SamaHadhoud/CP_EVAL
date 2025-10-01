#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
const int64 INF = (int64)4e18;

struct Job {
    int e;      // end residue
    int p;      // profit
    int64 l;    // length
};

static inline uint64_t pack_key(int s, int e, int p) {
    // s < 2e4, e < 2e4, p <= 5
    // pack into 64-bit: [ s (17b) | e (17b) | p (3b) ]
    return ( (uint64_t)s << 20 ) ^ ( (uint64_t)e << 3 ) ^ (uint64_t)p;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, n;
    if (!(cin >> T >> n)) return 0;

    // Deduplicate jobs by (s, e, p) keeping minimal l
    unordered_map<uint64_t, int64> bestL;
    bestL.reserve(n * 2);

    for (int i = 0; i < n; ++i) {
        int x; long long l; int p;
        cin >> x >> l >> p;
        int s = x;
        int e = (int)(( (long long)x + (l % T) ) % T);
        uint64_t key = pack_key(s, e, p);
        auto it = bestL.find(key);
        if (it == bestL.end()) bestL.emplace(key, l);
        else it->second = min(it->second, l);
    }

    // Build per-residue job lists and profit stats
    vector<vector<Job>> jobsByS(T);
    vector<int> activeS;
    vector<char> visS(T, 0);
    int K = 0;
    int maxP = 0;
    bool pUsed[6] = {false,false,false,false,false,false};

    for (auto &kv : bestL) {
        uint64_t key = kv.first;
        int64 l = kv.second;
        int p = (int)(key & 7ULL);
        int e = (int)((key >> 3) & ((1<<17)-1));
        int s = (int)((key >> 20) & ((1<<17)-1));
        jobsByS[s].push_back({e, p, l});
        if (!visS[s]) { visS[s] = 1; activeS.push_back(s); }
        maxP = max(maxP, p);
        pUsed[p] = true;
        K = std::gcd(K, p);
    }
    if (maxP == 0) {
        // No jobs effectively (shouldn't happen as n>=1 and p>=1), but handle anyway
        int q; cin >> q;
        while (q--) cout << 0 << '\n';
        return 0;
    }
    if (K == 0) K = maxP; // safety

    // Helper buffers for close-transform
    vector<int64> pref(T), suff(T+1);

    auto close_transform = [&](const vector<int64>& in, vector<int64>& out) {
        // out[s] = s + min( min_{r<=s}(in[r]-r), min_{r>s}(in[r]-r+T) )
        // Build prefix minima of (in[r]-r) and suffix minima of (in[r]-r+T)
        int64 cur = INF;
        for (int r = 0; r < T; ++r) {
            int64 a = in[r];
            if (a >= INF/2) {
                // leave 'cur' unchanged (min with INF is safe)
            } else {
                a -= r;
                if (a < cur) cur = a;
            }
            pref[r] = cur;
        }
        suff[T] = INF;
        cur = INF;
        for (int r = T-1; r >= 0; --r) {
            int64 a = in[r];
            if (a >= INF/2) {
                // keep cur
            } else {
                a = a - r + T;
                if (a < cur) cur = a;
            }
            suff[r] = min(suff[r+1], cur);
        }
        for (int s = 0; s < T; ++s) {
            int64 best = pref[s];
            int64 b = suff[s+1];
            if (b < best) best = b;
            if (best >= INF/2) out[s] = INF;
            else out[s] = (int64)s + best;
        }
    };

    // DP arrays:
    // closed: D_P fully "closed" vector (free at any residue after earning P)
    // raw[t]: raw accumulators for D_{P+t}, t in 1..maxP
    vector<int64> closed(T, INF), nextClosed(T, INF);
    vector<vector<int64>> raw(maxP + 1, vector<int64>(T, INF)); // index 1..maxP used

    // Initialize P = 0
    for (int r = 0; r < T; ++r) closed[r] = r; // free day r with 0 profit by waiting
    // Fill raw from P=0
    for (int s : activeS) {
        int64 base = closed[s];
        const auto& vec = jobsByS[s];
        for (const auto& jb : vec) {
            int t = jb.p, e = jb.e;
            int64 v = base + jb.l;
            if (v < raw[t][e]) raw[t][e] = v;
        }
    }

    // For period detection
    vector<vector<int64>> prevClosed(K, vector<int64>(T, INF));
    vector<char> havePrev(K, 0);

    // g(P) values collected
    vector<int64> gvals;
    auto min_of_vec = [&](const vector<int64>& a)->int64 {
        int64 m = INF;
        for (int i = 0; i < T; ++i) if (a[i] < m) m = a[i];
        return m;
    };
    gvals.reserve(200000);

    // Record g(0)
    gvals.push_back(min_of_vec(closed));

    // Period detection vars
    bool periodic = false;
    int64 C = -1;
    long long P0 = -1;

    // Store prevClosed for P = 0
    {
        int mod = 0 % K;
        prevClosed[mod] = closed;
        havePrev[mod] = 1;
    }

    // Main DP loop over P = 1, 2, ...
    long long P = 0;
    while (true) {
        // Move to next P
        ++P;
        // Compute closed for P from raw[1]
        close_transform(raw[1], nextClosed);
        closed.swap(nextClosed);

        // Record g(P)
        gvals.push_back(min_of_vec(closed));

        // Period detection when possible
        int mod = (int)(P % K);
        if (havePrev[mod]) {
            // Check D_P - D_{P-K} is constant across all residues
            int64 deltaC = LLONG_MIN;
            bool ok = true;
            for (int r = 0; r < T; ++r) {
                int64 a = closed[r];
                int64 b = prevClosed[mod][r];
                if (a >= INF/2 || b >= INF/2) { ok = false; break; }
                int64 d = a - b;
                if (deltaC == LLONG_MIN) deltaC = d;
                else if (d != deltaC) { ok = false; break; }
            }
            if (ok) {
                periodic = true;
                C = deltaC;
                P0 = P - K;
                break;
            }
        }
        prevClosed[mod] = closed;
        havePrev[mod] = 1;

        // Shift raw windows: raw[1] -> becomes D_{P+1}, so raw[1] is consumed.
        for (int t = 1; t < maxP; ++t) raw[t].swap(raw[t+1]);
        // Clear new tail raw[maxP]
        std::fill(raw[maxP].begin(), raw[maxP].end(), INF);

        // Add contributions from current closed (P) into raw
        for (int s : activeS) {
            int64 base = closed[s];
            const auto& vec = jobsByS[s];
            for (const auto& jb : vec) {
                int t = jb.p, e = jb.e;
                int64 v = base + jb.l;
                if (v < raw[t][e]) raw[t][e] = v;
            }
        }
    }

    // Prepare answering queries
    int q;
    cin >> q;

    // We have periodicity from P0, with period K and increment C.
    // We have gvals[0..(P0+K-1)] available since we broke at P = P0 + K
    long long border = P0 + K - 1;
    if (border >= (long long)gvals.size()) border = (long long)gvals.size() - 1;

    // Binary search helper on gvals[0..border]
    auto maxP_with_time_le = [&](int64 z)->long long {
        long long lo = 0, hi = border, ans = -1;
        while (lo <= hi) {
            long long mid = (lo + hi) >> 1;
            if (gvals[(size_t)mid] <= z) { ans = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        return ans; // -1 means can't earn even 0? But g(0)=0 so for z>=0 it's >=0.
    };

    // Answer each query
    while (q--) {
        int64 z; cin >> z;
        if (!periodic) {
            // Shouldn't happen with this model, but fallback: scan available gvals
            long long ans = -1;
            long long lo = 0, hi = (long long)gvals.size() - 1;
            while (lo <= hi) {
                long long mid = (lo + hi) >> 1;
                if (gvals[(size_t)mid] <= z) { ans = mid; lo = mid + 1; }
                else hi = mid - 1;
            }
            if (ans < 0) ans = 0; // at least P=0 with time 0
            cout << ans << '\n';
            continue;
        }

        if (z <= gvals[(size_t)border]) {
            long long ans = maxP_with_time_le(z);
            if (ans < 0) ans = 0;
            cout << ans << '\n';
            continue;
        }

        // Tail: use periodicity
        // base at P0
        int64 gP0 = gvals[(size_t)P0];
        // Number of full periods we can add
        long long qper = (C == 0 ? 0 : (long long)((z - gP0) / C));
        if (qper < 0) qper = 0; // safety
        long long baseProfit = P0 + qper * (long long)K;
        int64 baseTime = gP0 + qper * C;
        // Try adding extra t in [0..K-1]
        long long bestAdd = 0;
        for (int t = 0; t < K; ++t) {
            int64 need = gvals[(size_t)(P0 + t)];
            if (need <= baseTime && t > bestAdd) bestAdd = t;
            else if (need > baseTime) {
                // we can still try to see if need <= z by adding within same period offset
                // But since baseTime = gP0 + qper*C, need = g(P0+t), so we need need <= baseTime + leftover
                // We'll handle below with leftover.
                ;
            }
        }
        int64 leftover = z - baseTime;
        // Now find the largest t with g(P0 + t) <= gP0 + leftover
        for (int t = 0; t < K; ++t) {
            if (gvals[(size_t)(P0 + t)] <= gP0 + leftover) {
                if (t > bestAdd) bestAdd = t;
            }
        }
        cout << (baseProfit + bestAdd) << '\n';
    }
    return 0;
}