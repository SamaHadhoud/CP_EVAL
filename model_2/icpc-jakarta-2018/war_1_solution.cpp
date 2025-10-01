#include <bits/stdc++.h>
using namespace std;

struct Point { long long x, y; };
struct Vec { long long x, y; };
struct Wall { long long x1, x2, y; int lid, rid; };
struct Interval {
    long double L, R;
    int lid, rid;
};
static const long double EPS = 1e-12L;

inline Vec make_vec(const Point& a, const Point& b) { return {a.x - b.x, a.y - b.y}; }
inline Vec neg_vec(const Vec& v) { return {-v.x, -v.y}; }

// Sort by polar angle in [0, 2π): upper half-plane first (y>0 or y==0 & x>=0), then by CCW.
inline int half(const Vec& v) { return (v.y > 0 || (v.y == 0 && v.x >= 0)) ? 0 : 1; }
inline bool angleLess(const Vec& a, const Vec& b) {
    int ha = half(a), hb = half(b);
    if (ha != hb) return ha < hb;
    __int128 cross = (__int128)a.x * b.y - (__int128)a.y * b.x;
    if (cross != 0) return cross > 0; // a before b if a->b is CCW
    return false; // same angle
}

inline long long count_inclusive_range(const vector<Vec>& arr, const Vec& low, const Vec& high) {
    if (arr.empty()) return 0;
    bool wrap = !angleLess(low, high);
    auto l = lower_bound(arr.begin(), arr.end(), low, angleLess);
    auto r = upper_bound(arr.begin(), arr.end(), high, angleLess);
    if (!wrap) return (long long)(r - l);
    return (long long)((arr.end() - l) + (r - arr.begin()));
}

inline long long count_open_range(const vector<Vec>& arr, const Vec& low, const Vec& high) {
    if (arr.empty()) return 0;
    bool wrap = !angleLess(low, high);
    auto l = upper_bound(arr.begin(), arr.end(), low, angleLess);   // exclude =low
    auto r = lower_bound(arr.begin(), arr.end(), high, angleLess);  // exclude =high
    if (!wrap) return (long long)(r - l);
    return (long long)((arr.end() - l) + (r - arr.begin()));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, Q;
    if (!(cin >> N >> M >> Q)) return 0;
    vector<Point> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i].x >> A[i].y;
    vector<Wall> W(M);
    vector<Point> E(2 * M);
    for (int j = 0; j < M; ++j) {
        cin >> W[j].x1 >> W[j].x2 >> W[j].y;
        W[j].lid = 2 * j;
        W[j].rid = 2 * j + 1;
        E[W[j].lid] = {W[j].x1, W[j].y};
        E[W[j].rid] = {W[j].x2, W[j].y};
    }

    // Per-endpoint direction lists: right endpoints use inclusive (<=), left endpoints use strict (<)
    vector<vector<Vec>> rights(2 * M), lefts(2 * M);

    // Build intervals per artillery, merge, then add contributions to endpoints
    for (int i = 0; i < N; ++i) {
        vector<Interval> v;
        v.reserve(M);
        for (int j = 0; j < M; ++j) {
            if (A[i].y <= W[j].y) continue; // this wall never blocks this artillery
            long double denom = (long double)A[i].y - (long double)W[j].y; // > 0
            long double L = ((long double)W[j].x1 * (long double)A[i].y - (long double)W[j].y * (long double)A[i].x) / denom;
            long double R = ((long double)W[j].x2 * (long double)A[i].y - (long double)W[j].y * (long double)A[i].x) / denom;
            // since x1 < x2 and denom > 0, L < R
            v.push_back({L, R, W[j].lid, W[j].rid});
        }
        if (v.empty()) continue;
        sort(v.begin(), v.end(), [](const Interval& a, const Interval& b){
            if (fabsl(a.L - b.L) > EPS) return a.L < b.L;
            return a.R < b.R;
        });
        long double curL = v[0].L, curR = v[0].R;
        int eL = v[0].lid, eR = v[0].rid;
        for (size_t t = 1; t < v.size(); ++t) {
            if (v[t].L <= curR + EPS) {
                if (v[t].R > curR + EPS) {
                    curR = v[t].R;
                    eR = v[t].rid;
                }
            } else {
                // emit merged [curL, curR] with endpoints eL (left) and eR (right)
                Vec vL = make_vec(A[i], E[eL]); // vector from endpoint to artillery
                Vec vR = make_vec(A[i], E[eR]);
                lefts[eL].push_back(vL);
                rights[eR].push_back(vR);
                curL = v[t].L; curR = v[t].R; eL = v[t].lid; eR = v[t].rid;
            }
        }
        Vec vL = make_vec(A[i], E[eL]);
        Vec vR = make_vec(A[i], E[eR]);
        lefts[eL].push_back(vL);
        rights[eR].push_back(vR);
    }

    // Sort direction lists by angle
    for (int e = 0; e < 2 * M; ++e) {
        sort(rights[e].begin(), rights[e].end(), angleLess);
        sort(lefts[e].begin(), lefts[e].end(), angleLess);
    }

    // Process queries
    for (int qi = 0; qi < Q; ++qi) {
        Point w;
        cin >> w.x >> w.y;
        long long blocked = 0;
        for (int e = 0; e < 2 * M; ++e) {
            Vec D = make_vec(w, E[e]);   // from endpoint E to watchtower w
            Vec low = neg_vec(D);        // angle(w|E)+π
            // Rights: inclusive [low, D]
            blocked += count_inclusive_range(rights[e], low, D);
            // Lefts: open (low, D)
            blocked -= count_open_range(lefts[e], low, D);
        }
        long long visible = (long long)N - blocked;
        if (visible < 0) visible = 0; // safety
        cout << visible << '\n';
    }
    return 0;
}