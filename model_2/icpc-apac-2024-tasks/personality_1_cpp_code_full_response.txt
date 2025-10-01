```cpp
#include <bits/stdc++.h>
using namespace std;

// Solve as per editorial:
// - k=1: simple O(n*m) sweep with last[j][c] -> best a for each b.
// - k>=2: bitsets over students per (column, letter). For each b, use
//   residues modulo P=k-1 and a two-hit accumulator per residue to get candidates,
//   then verify candidates in decreasing a.

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    vector<string> S(n);
    for (int i = 0; i < n; ++i) cin >> S[i];

    // Precompute letter counts per student for quick upper bound in verification.
    vector<array<int, 26>> cnt(n);
    for (int i = 0; i < n; ++i) {
        cnt[i].fill(0);
        for (int j = 0; j < m; ++j) {
            char ch = S[i][j];
            if (ch != '.') cnt[i][ch - 'A']++;
        }
    }

    // Special case k = 1
    if (k == 1) {
        // last[j][c] = largest index of student seen so far with letter c at column j (1-based index)
        vector<int> last((size_t)m * 26, 0);
        for (int b = 1; b <= n; ++b) {
            const string &Sb = S[b - 1];
            int best_a = 0;
            for (int j = 0; j < m; ++j) {
                char ch = Sb[j];
                if (ch == '.') continue;
                int idx = j * 26 + (ch - 'A');
                if (last[idx] > best_a) best_a = last[idx];
            }
            if (best_a > 0) {
                cout << best_a << " " << b << "\n";
                return 0;
            }
            // update last for this student
            for (int j = 0; j < m; ++j) {
                char ch = Sb[j];
                if (ch == '.') continue;
                last[j * 26 + (ch - 'A')] = b;
            }
        }
        cout << -1 << "\n";
        return 0;
    }

    // k >= 2
    int P = k - 1;

    // Precompute residues: list of columns for each residue r = j % P
    vector<vector<int>> cols(P);
    cols.assign(P, {});
    for (int j = 0; j < m; ++j) cols[j % P].push_back(j);

    // Build bitsets B over students (dimension n) for each (column j, letter c)
    // Flattened layout: B[(j*26 + c)*W + w] stores 64-bit word w
    int W = (n + 63) >> 6;
    vector<uint64_t> B((size_t)m * 26 * W, 0ULL);

    // Fill B
    for (int j = 0; j < m; ++j) {
        for (int i = 0; i < n; ++i) {
            char ch = S[i][j];
            if (ch == '.') continue;
            int c = ch - 'A';
            size_t base = ((size_t)j * 26 + c) * (size_t)W;
            int word = i >> 6, bit = i & 63;
            B[base + word] |= (1ULL << bit);
        }
    }

    // Temporary bitsets used per b
    vector<uint64_t> candidate(W), hit1(W), hit2(W);

    // Helper lambda for quick upper bound on possible matches
    auto ub_possible = [&](int a, int b) -> bool {
        int ub = 0;
        for (int c = 0; c < 26; ++c) {
            ub += min(cnt[a][c], cnt[b][c]);
            if (ub >= k) return true; // early exit
        }
        return ub >= k;
    };

    // Verify exact count of matches between Sa and Sb (early stop at k)
    auto verify = [&](int a, int b) -> bool {
        if (!ub_possible(a, b)) return false;
        const string &Sa = S[a];
        const string &Sb = S[b];
        int match = 0;
        for (int j = 0; j < m; ++j) {
            char cb = Sb[j];
            if (cb != '.' && Sa[j] == cb) {
                if (++match >= k) return true;
            }
        }
        return false;
    };

    for (int b = 2; b <= n; ++b) {
        int bidx = b - 1; // 0-based student index
        int prevBits = b - 1; // number of previous students
        int Wb = (prevBits + 63) >> 6;
        // Mask for the last word (only bits < b-1 are valid)
        int r = prevBits & 63;
        uint64_t lastMask = (Wb == 0 ? 0ULL : (r == 0 ? ~0ULL : ((1ULL << r) - 1)));

        // Reset candidate
        if (Wb > 0) memset(candidate.data(), 0, sizeof(uint64_t) * Wb);

        // For each residue class, accumulate students with at least two matches in that residue
        for (int rr = 0; rr < P; ++rr) {
            if (Wb > 0) {
                memset(hit1.data(), 0, sizeof(uint64_t) * Wb);
                memset(hit2.data(), 0, sizeof(uint64_t) * Wb);
            }
            const vector<int> &cr = cols[rr];
            for (int jj = 0; jj < (int)cr.size(); ++jj) {
                int j = cr[jj];
                char ch = S[bidx][j];
                if (ch == '.') continue;
                int c = ch - 'A';
                const uint64_t *Bj = &B[((size_t)j * 26 + c) * (size_t)W];
                // Saturating two-of-many accumulator
                for (int w = 0; w < Wb; ++w) {
                    uint64_t wv = Bj[w];
                    if (w == Wb - 1) wv &= lastMask;
                    uint64_t x = hit1[w] & wv;
                    hit2[w] |= x;
                    hit1[w] |= wv;
                }
            }
            // Merge into candidate
            for (int w = 0; w < Wb; ++w) candidate[w] |= hit2[w];
        }

        // If we have candidates, check them in decreasing a to respect tie-breaking
        if (Wb > 0) {
            for (int w = Wb - 1; w >= 0; --w) {
                uint64_t ww = candidate[w];
                if (w == Wb - 1) ww &= lastMask;
                while (ww) {
                    int bit = 63 - __builtin_clzll(ww);
                    int aidx = (w << 6) + bit; // 0-based index for student a
                    int a = aidx + 1; // 1-based
                    // Verify exact similarity
                    if (verify(a - 1, b - 1)) {
                        cout << a << " " << b << "\n";
                        return 0;
                    }
                    ww ^= (1ULL << bit);
                }
            }
        }
        // else no candidates for this b, continue
    }

    cout << -1 << "\n";
    return 0;
}
```