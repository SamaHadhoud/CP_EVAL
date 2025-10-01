```cpp
#include <bits/stdc++.h>
using namespace std;

struct Op {
    int t, x, a;
};

struct Comp {
    vector<int> nodes;         // nodes in this clean component
    int cnt[2] = {0, 0};       // counts by parity
    int off[2] = {0, 0};       // current offsets by parity (mod 360)
    vector<int> adjS;          // adjacent S-nodes (original ids)
    vector<uint16_t> pref16[2]; // prefix sums (size 361) for small comps
    vector<uint32_t> pref32[2]; // prefix sums (size 361) for large comps
    inline int size() const { return cnt[0] + cnt[1]; }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    vector<vector<int>> g(N + 1);
    for (int i = 0; i < N - 1; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // Compute parity p[u] (0/1) by BFS from root 1
    vector<int> p(N + 1, -1);
    queue<int> q;
    p[1] = 0;
    q.push(1);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (p[v] == -1) {
                p[v] = p[u] ^ 1;
                q.push(v);
            }
        }
    }
    
    int Q;
    cin >> Q;
    vector<Op> ops(Q);
    for (int i = 0; i < Q; ++i) {
        int t; cin >> t;
        if (t == 1 || t == 2) {
            int x; cin >> x;
            ops[i] = {t, x, 0};
        } else {
            int x, a; cin >> x >> a;
            ops[i] = {t, x, a};
        }
    }

    // State
    vector<int> ang(N + 1, 0);            // angles modulo 360
    vector<char> on(N + 1, 1);            // on-board flags
    long long T = 0;                      // total sum of angles

    // For blocks
    const int B = 800;                    // block size
    vector<char> inS(N + 1, 0);           // nodes toggled in this block
    vector<int> compOf(N + 1, -1);        // node -> component id (for clean comps)

    // Overlay graph structures for S
    vector<vector<int>> sAdj(N + 1);      // adjacency for S nodes (overlay ids)

    // Visited stamps
    vector<int> sStamp(N + 1, 0);
    int curStamp = 1;

    auto mod360 = [](int x)->int { x %= 360; if (x < 0) x += 360; return x; };

    for (int blkStart = 0; blkStart < Q; blkStart += B) {
        int blkEnd = min(Q, blkStart + B);

        // Determine S = nodes toggled in this block
        vector<int> Slist;
        for (int i = blkStart; i < blkEnd; ++i) {
            if (ops[i].t == 1 || ops[i].t == 2) {
                int x = ops[i].x;
                if (!inS[x]) {
                    inS[x] = 1;
                    Slist.push_back(x);
                }
            }
        }

        // Build clean components among nodes that are on and not in S
        fill(compOf.begin(), compOf.end(), -1);
        vector<Comp> comps;
        vector<int> order; // temp BFS container

        for (int u = 1; u <= N; ++u) {
            if (on[u] && !inS[u] && compOf[u] == -1) {
                comps.push_back(Comp());
                int cid = (int)comps.size() - 1;
                comps[cid].nodes.clear();

                // BFS to collect nodes of the component
                order.clear();
                order.push_back(u);
                compOf[u] = cid;
                for (size_t it = 0; it < order.size(); ++it) {
                    int x = order[it];
                    comps[cid].nodes.push_back(x);
                    for (int v : g[x]) {
                        if (on[v] && !inS[v] && compOf[v] == -1) {
                            compOf[v] = cid;
                            order.push_back(v);
                        }
                    }
                }

                // Count parity
                comps[cid].cnt[0] = comps[cid].cnt[1] = 0;
                for (int v : comps[cid].nodes) {
                    comps[cid].cnt[p[v]]++;
                }

                // Build histogram then prefix sums (free histogram afterward)
                // We'll use 32-bit hist locally, then convert to 16/32-bit prefix as needed
                vector<uint32_t> hist0(360, 0), hist1(360, 0);
                for (int v : comps[cid].nodes) {
                    int par = p[v];
                    int a = ang[v];
                    if (par == 0) hist0[a]++; else hist1[a]++;
                }
                // Build prefix sums per parity with chosen width
                for (int par = 0; par < 2; ++par) {
                    int cntPar = comps[cid].cnt[par];
                    if (cntPar <= 65535) {
                        comps[cid].pref16[par].assign(361, 0);
                        if (par == 0) {
                            for (int i = 0; i < 360; ++i) {
                                comps[cid].pref16[par][i + 1] = (uint16_t)(comps[cid].pref16[par][i] + (uint16_t)hist0[i]);
                            }
                        } else {
                            for (int i = 0; i < 360; ++i) {
                                comps[cid].pref16[par][i + 1] = (uint16_t)(comps[cid].pref16[par][i] + (uint16_t)hist1[i]);
                            }
                        }
                    } else {
                        comps[cid].pref32[par].assign(361, 0);
                        if (par == 0) {
                            for (int i = 0; i < 360; ++i) {
                                comps[cid].pref32[par][i + 1] = comps[cid].pref32[par][i] + hist0[i];
                            }
                        } else {
                            for (int i = 0; i < 360; ++i) {
                                comps[cid].pref32[par][i + 1] = comps[cid].pref32[par][i] + hist1[i];
                            }
                        }
                    }
                }
                comps[cid].off[0] = comps[cid].off[1] = 0;
            }
        }

        // Build overlay adjacency:
        // - Clear sAdj for S nodes only
        for (int s : Slist) sAdj[s].clear();
        // - For each comp, clear adjS
        for (auto &C : comps) C.adjS.clear();

        // Edges: S-S and S-Comp
        for (int s : Slist) {
            for (int v : g[s]) {
                if (inS[v]) {
                    sAdj[s].push_back(v);
                } else if (on[v]) {
                    int cid = compOf[v];
                    if (cid != -1) {
                        int overlayCompId = N + 1 + cid;
                        sAdj[s].push_back(overlayCompId);
                        comps[cid].adjS.push_back(s);
                    }
                }
            }
        }
        // Deduplicate adjacency
        for (int s : Slist) {
            auto &adj = sAdj[s];
            sort(adj.begin(), adj.end());
            adj.erase(unique(adj.begin(), adj.end()), adj.end());
        }
        for (auto &C : comps) {
            auto &adj = C.adjS;
            sort(adj.begin(), adj.end());
            adj.erase(unique(adj.begin(), adj.end()), adj.end());
        }

        // Visited stamps for components (per block)
        vector<int> cStamp(comps.size(), 0);

        auto rangeCount = [&](const Comp &C, int par, int L, int R) -> uint32_t {
            // Count of indices in circular range [L..R] using prefix sums
            if (L < 0) L += 360;
            if (R < 0) R += 360;
            if (L >= 360) L -= 360;
            if (R >= 360) R -= 360;
            if (C.pref32[par].empty()) {
                const auto &pref = C.pref16[par];
                if (L <= R) {
                    return (uint32_t)(pref[R + 1] - pref[L]);
                } else {
                    return (uint32_t)((pref[360] - pref[L]) + pref[R + 1]);
                }
            } else {
                const auto &pref = C.pref32[par];
                if (L <= R) {
                    return (uint32_t)(pref[R + 1] - pref[L]);
                } else {
                    return (uint32_t)((pref[360] - pref[L]) + pref[R + 1]);
                }
            }
        };

        // Process operations in this block
        for (int i = blkStart; i < blkEnd; ++i) {
            const Op &op = ops[i];
            if (op.t == 1) {
                // remove
                on[op.x] = 0;
            } else if (op.t == 2) {
                // insert
                on[op.x] = 1;
            } else {
                int x = op.x, alpha = op.a;
                if (alpha == 0) {
                    cout << 0 << '\n';
                    continue;
                }

                // Prepare BFS across overlay graph
                ++curStamp;
                vector<int> visS;
                vector<int> visC;
                queue<int> qq;

                if (inS[x]) {
                    // start from S node x
                    // guaranteed on[x] == 1 by problem statement
                    sStamp[x] = curStamp;
                    visS.push_back(x);
                    qq.push(x);
                } else {
                    // start from component containing x
                    int cid = compOf[x];
                    // x must be on-board and not in S, guaranteed by constraints
                    int oid = N + 1 + cid;
                    cStamp[cid] = curStamp;
                    visC.push_back(cid);
                    qq.push(oid);
                }

                while (!qq.empty()) {
                    int u = qq.front(); qq.pop();
                    if (u <= N) {
                        // S node
                        int s = u;
                        // traverse neighbors
                        for (int v : sAdj[s]) {
                            if (v <= N) {
                                int t = v;
                                if (!on[t]) continue;          // S nodes: traverse only if on
                                if (sStamp[t] == curStamp) continue;
                                sStamp[t] = curStamp;
                                visS.push_back(t);
                                qq.push(t);
                            } else {
                                int cid = v - (N + 1);
                                if (cStamp[cid] == curStamp) continue;
                                cStamp[cid] = curStamp;
                                visC.push_back(cid);
                                qq.push(v);
                            }
                        }
                    } else {
                        // Component node
                        int cid = u - (N + 1);
                        // traverse to adjacent S nodes that are currently on
                        for (int s : comps[cid].adjS) {
                            if (!on[s]) continue;
                            if (sStamp[s] == curStamp) continue;
                            sStamp[s] = curStamp;
                            visS.push_back(s);
                            qq.push(s);
                        }
                    }
                }

                // Compute energy
                long long rotCount = (long long)visS.size();
                for (int cid : visC) rotCount += comps[cid].size();
                long long energy = 1LL * alpha * rotCount;
                cout << energy << '\n';

                // Update total sum T and offsets/angles
                int px = p[x];
                // Components
                for (int cid : visC) {
                    Comp &C = comps[cid];
                    // +alpha for parity px
                    int off_px = C.off[px];
                    int Lp = mod360(360 - alpha - off_px);
                    int Rp = mod360(359 - off_px);
                    uint32_t wrapsPlus = rangeCount(C, px, Lp, Rp);

                    // -alpha for parity 1-px
                    int py = px ^ 1;
                    int off_py = C.off[py];
                    uint32_t wrapsMinus = 0;
                    if (alpha > 0) {
                        int Lm = mod360(0 - off_py);
                        int Rm = mod360(alpha - 1 - off_py);
                        wrapsMinus = rangeCount(C, py, Lm, Rm);
                    }

                    T += 1LL * alpha * C.cnt[px] - 360LL * (long long)wrapsPlus;
                    T += -1LL * alpha * C.cnt[py] + 360LL * (long long)wrapsMinus;

                    C.off[px] = (off_px + alpha) % 360;
                    C.off[py] = mod360(off_py - alpha);
                }
                // S nodes
                for (int s : visS) {
                    if (p[s] == px) {
                        int v = ang[s] + alpha;
                        if (v >= 360) {
                            ang[s] = v - 360;
                            T += (long long)alpha - 360;
                        } else {
                            ang[s] = v;
                            T += (long long)alpha;
                        }
                    } else {
                        if (ang[s] >= alpha) {
                            ang[s] -= alpha;
                            T += -(long long)alpha;
                        } else {
                            ang[s] = ang[s] - alpha + 360;
                            T += 360 - (long long)alpha;
                        }
                    }
                }
            }
        }

        // Materialize offsets into angles for next block
        for (auto &C : comps) {
            if (C.off[0] == 0 && C.off[1] == 0) continue;
            int off0 = C.off[0], off1 = C.off[1];
            for (int u : C.nodes) {
                int add = (p[u] == 0) ? off0 : off1;
                if (add) {
                    int v = ang[u] + add;
                    v %= 360;
                    ang[u] = v;
                }
            }
            // offsets reset with component destruction at end of scope
        }

        // Clear inS flags for next block
        for (int s : Slist) inS[s] = 0;
    }

    // Output final total sum
    cout << T << '\n';
    return 0;
}
```