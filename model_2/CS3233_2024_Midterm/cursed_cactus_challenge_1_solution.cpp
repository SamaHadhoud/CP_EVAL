#include <bits/stdc++.h>
using namespace std;

static const long long MOD = 998244353;

struct Trip {
    long long c, s1, s2;
    Trip(long long c_=0, long long s1_=0, long long s2_=0): c(c_), s1(s1_), s2(s2_) {}
};

inline long long norm(long long x){ x%=MOD; if(x<0) x+=MOD; return x; }

inline Trip addT(const Trip &A, const Trip &B){
    Trip R;
    R.c  = A.c  + B.c;  if (R.c  >= MOD) R.c  -= MOD;
    R.s1 = A.s1 + B.s1; if (R.s1 >= MOD) R.s1 -= MOD;
    R.s2 = A.s2 + B.s2; if (R.s2 >= MOD) R.s2 -= MOD;
    return R;
}

// Disjoint union: (c, s1, s2) with (c', s1', s2')
inline Trip mulT(const Trip &A, const Trip &B){
    Trip R;
    long long cA=A.c, cB=B.c, s1A=A.s1, s1B=B.s1, s2A=A.s2, s2B=B.s2;
    R.c  = (cA * cB) % MOD;
    R.s1 = ( (s1A * cB) + (cA * s1B) ) % MOD;
    long long term1 = (s2A * cB) % MOD;
    long long term2 = (cA * s2B) % MOD;
    long long term3 = (2 * ( (s1A * s1B) % MOD )) % MOD;
    R.s2 = (term1 + term2) % MOD;
    R.s2 = (R.s2 + term3) % MOD;
    return R;
}

inline Trip withVertex(const Trip &A, long long a){
    long long aa = a % MOD;
    long long a2 = (aa * aa) % MOD;
    Trip V(1, aa, a2);
    return mulT(A, V);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if(!(cin >> n >> m)) return 0;
    vector<long long> a(n);
    for(int i=0;i<n;i++){
        long long x; cin >> x;
        a[i] = x % MOD;
    }
    vector<vector<pair<int,int>>> g(n);
    g.reserve(n);
    for(int i=0;i<m;i++){
        int u,v; cin >> u >> v; --u; --v;
        g[u].emplace_back(v, i);
        g[v].emplace_back(u, i);
    }

    // DFS structures
    vector<int> parent(n, -1), parentEdge(n, -1), depth(n, -1);
    vector<char> state(n, 0), inStack(n, 0);
    vector<char> isCycleEdge(m, 0);

    vector<vector<int>> cycles;                 // each cycle is [anchor, x2, ..., xk]
    vector<vector<int>> anchoredCycleIds(n);    // cycle indices anchored at vertex

    struct Frame { int v, p, ei; };
    vector<Frame> st;
    int root = 0;
    st.push_back({root, -1, 0});

    // DP arrays
    vector<Trip> dp0(n), dp1(n);

    // Iterative DFS with postorder DP
    while(!st.empty()){
        Frame &fr = st.back();
        int v = fr.v;
        if(state[v] == 0){
            state[v] = 1;
            inStack[v] = 1;
            parent[v] = fr.p;
            depth[v] = (fr.p == -1 ? 0 : depth[fr.p] + 1);
            // parentEdge[v] is already set when pushed (except root)
        }

        if(fr.ei < (int)g[v].size()){
            auto [to, eid] = g[v][fr.ei++];
            if(to == parent[v]) continue;
            if(state[to] == 0){
                parent[to] = v;
                parentEdge[to] = eid;
                st.push_back({to, v, 0});
                continue;
            }else if(inStack[to] && depth[to] < depth[v]){
                // Found a back-edge v -> to (to is ancestor)
                // Build cycle anchored at 'to'
                vector<int> cyc;
                cyc.push_back(to);
                int cur = v;
                isCycleEdge[eid] = 1; // back edge
                while(cur != to){
                    cyc.push_back(cur);
                    int pe = parentEdge[cur];
                    isCycleEdge[pe] = 1; // tree edge along the cycle path
                    cur = parent[cur];
                }
                // Now cyc = [to, v, parent[v], ..., child_of_to], reverse segment [1..end]
                reverse(cyc.begin()+1, cyc.end());
                int cid = (int)cycles.size();
                cycles.push_back(move(cyc));
                anchoredCycleIds[to].push_back(cid);
            }
            continue;
        }

        // Exit v: aggregate DP
        Trip P0(1,0,0), P1(1,0,0);

        // Combine tree children (edges to children not in cycles)
        for(auto &e : g[v]){
            int u = e.first, eid = e.second;
            if(parent[u] == v){
                if(!isCycleEdge[eid]){
                    Trip sum = addT(dp0[u], dp1[u]);
                    P0 = mulT(P0, sum);
                    P1 = mulT(P1, dp0[u]);
                }
            }
        }

        // Combine anchored cycles at v
        for(int cid : anchoredCycleIds[v]){
            const vector<int> &cyc = cycles[cid]; // cyc[0] == v
            int k = (int)cyc.size();
            // v = 0
            Trip cur0 = dp0[cyc[1]];
            Trip cur1 = dp1[cyc[1]];
            for(int i=2;i<k;i++){
                int x = cyc[i];
                Trip new0 = mulT(addT(cur0, cur1), dp0[x]);
                Trip new1 = mulT(cur0, dp1[x]);
                cur0 = new0; cur1 = new1;
            }
            Trip C0 = addT(cur0, cur1);
            // v = 1
            cur0 = dp0[cyc[1]];
            cur1 = Trip(0,0,0);
            for(int i=2;i<k;i++){
                int x = cyc[i];
                Trip new0 = mulT(addT(cur0, cur1), dp0[x]);
                Trip new1 = mulT(cur0, dp1[x]);
                cur0 = new0; cur1 = new1;
            }
            Trip C1 = cur0;

            P0 = mulT(P0, C0);
            P1 = mulT(P1, C1);
        }

        dp0[v] = P0;
        dp1[v] = withVertex(P1, a[v]);

        state[v] = 2;
        inStack[v] = 0;
        st.pop_back();
    }

    Trip total = addT(dp0[root], dp1[root]);
    cout << (total.s2 % MOD) << '\n';
    return 0;
}