```cpp
#include <bits/stdc++.h>
using namespace std;

struct Pt {
    double x, y;
    Pt() {}
    Pt(double _x, double _y): x(_x), y(_y) {}
    Pt operator+(const Pt& o) const { return Pt(x + o.x, y + o.y); }
    Pt operator-(const Pt& o) const { return Pt(x - o.x, y - o.y); }
    Pt operator*(double k) const { return Pt(x * k, y * k); }
    Pt operator/(double k) const { return Pt(x / k, y / k); }
};

static inline double dot(const Pt& a, const Pt& b){ return a.x*b.x + a.y*b.y; }
static inline double norm2(const Pt& a){ return dot(a, a); }
static inline double norm(const Pt& a){ return sqrt(norm2(a)); }
static inline Pt perp(const Pt& a){ return Pt(-a.y, a.x); }

struct Circle { Pt c; double r; };

const double EPS = 1e-9;
const double PT_EPS = 1e-9;
const double PT_EPS2 = PT_EPS * PT_EPS;

int N;
double xL, yL, xR, yR;
Pt S, Tt;
vector<Circle> C;

vector<Pt> nodes;
vector<vector<pair<int,double>>> adj;
int idS, idT;
int idCorners[4];
Pt corners[4];

int addNode(const Pt& p){
    for (int i = 0; i < (int)nodes.size(); ++i){
        Pt d = p - nodes[i];
        if (norm2(d) <= PT_EPS2) return i;
    }
    nodes.push_back(p);
    adj.emplace_back();
    return (int)nodes.size() - 1;
}

double segCircleDist(const Pt& a, const Pt& b, const Circle& circ){
    Pt d = b - a;
    double dd = norm2(d);
    if (dd < 1e-24) return norm(circ.c - a);
    double t = dot(circ.c - a, d) / dd;
    if (t < 0) t = 0;
    else if (t > 1) t = 1;
    Pt x = a + d * t;
    return norm(circ.c - x);
}

bool isFreeSegment(const Pt& a, const Pt& b){
    for (int i = 0; i < N; ++i){
        double dist = segCircleDist(a, b, C[i]);
        if (dist + EPS < C[i].r) return false;
    }
    return true;
}

void addEdgeIds(int u, int v){
    if (u == v) return;
    Pt a = nodes[u], b = nodes[v];
    double len2 = norm2(b - a);
    if (len2 < 1e-20) return;
    if (!isFreeSegment(a, b)) return;
    double w = sqrt(len2);
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
}

void tryConnectToCorners(int id, const Pt& A, const Pt& n){
    double rhs = dot(n, A);
    for (int k = 0; k < 4; ++k){
        Pt R = corners[k];
        if (dot(n, R) + EPS >= rhs){ // R in outer half-plane
            addEdgeIds(id, idCorners[k]);
        }
    }
}

void addTangentsFromPoint(const Pt& P, int idP, int i){
    Pt Cc = C[i].c;
    double r = C[i].r;
    Pt u = P - Cc;
    double d = norm(u);
    if (d <= r + 1e-12) return; // should not happen by constraints
    Pt uhat = u / d;
    Pt vhat = perp(uhat);
    double h = r / d;
    h = max(-1.0, min(1.0, h));
    double tval = sqrt(max(0.0, 1.0 - h*h));
    // normals
    Pt n1 = uhat * h + vhat * tval;
    Pt n2 = uhat * h - vhat * tval;
    Pt T1 = Cc + n1 * r;
    Pt T2 = Cc + n2 * r;
    int id1 = addNode(T1);
    int id2 = addNode(T2);
    // Edge from P to tangency points
    addEdgeIds(idP, id1);
    addEdgeIds(idP, id2);
    // Connect tangency points to corners (outer half-plane)
    tryConnectToCorners(id1, T1, n1);
    tryConnectToCorners(id2, T2, n2);
    // Optionally connect tangency points to S and T directly if safe (more options)
    addEdgeIds(id1, idS);
    addEdgeIds(id2, idS);
    addEdgeIds(id1, idT);
    addEdgeIds(id2, idT);
}

void addCirclePairBitangents(int i, int j){
    Pt Ci = C[i].c, Cj = C[j].c;
    double ri = C[i].r, rj = C[j].r;
    Pt u = Cj - Ci;
    double d = norm(u);
    if (d <= ri + rj + 1e-12) return; // though constraints ensure >
    Pt ex = u / d;
    Pt ey = perp(ex);
    for (int sgn = +1; sgn >= -1; sgn -= 2){ // +1 outer, -1 inner
        double rjp = sgn * rj;
        double h = (ri - rjp) / d;
        h = max(-1.0, min(1.0, h));
        double base = sqrt(max(0.0, 1.0 - h*h));
        for (int tsgn = +1; tsgn >= -1; tsgn -= 2){
            Pt n = ex * h + ey * (tsgn * base); // unit normal with dot(n, ex) = h
            Pt Ti = Ci + n * ri;
            Pt Tj = Cj + n * rjp; // if inner, rjp negative; it's fine
            int idi = addNode(Ti);
            int idj = addNode(Tj);
            addEdgeIds(idi, idj);
            // corner connections:
            // For circle i, normal at Ti is n
            tryConnectToCorners(idi, Ti, n);
            // For circle j, normal at Tj is (Tj - Cj)/rj = (rjp * n)/rj = sgn * n
            Pt nj = n * (double)sgn;
            tryConnectToCorners(idj, Tj, nj);
            // Optional connections to S/T
            addEdgeIds(idi, idS);
            addEdgeIds(idi, idT);
            addEdgeIds(idj, idS);
            addEdgeIds(idj, idT);
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n >> xL >> yL >> xR >> yR;
    N = n;
    double xs, ys, xt, yt;
    cin >> xs >> ys;
    cin >> xt >> yt;
    S = Pt(xs, ys);
    Tt = Pt(xt, yt);

    C.resize(N);
    for (int i = 0; i < N; ++i){
        double xi, yi, ri;
        cin >> xi >> yi >> ri;
        C[i] = {Pt(xi, yi), ri};
    }

    // Init nodes: S, T, corners
    idS = addNode(S);
    idT = addNode(Tt);
    corners[0] = Pt(xL, yL);
    corners[1] = Pt(xL, yR);
    corners[2] = Pt(xR, yL);
    corners[3] = Pt(xR, yR);
    for (int k = 0; k < 4; ++k) idCorners[k] = addNode(corners[k]);

    // Quick direct check
    if (isFreeSegment(S, Tt)){
        cout << 0 << "\n";
        return 0;
    }

    // Optional connections: S/T <-> corners, corners <-> corners
    for (int k = 0; k < 4; ++k){
        addEdgeIds(idS, idCorners[k]);
        addEdgeIds(idT, idCorners[k]);
    }
    for (int a = 0; a < 4; ++a){
        for (int b = a+1; b < 4; ++b){
            addEdgeIds(idCorners[a], idCorners[b]);
        }
    }

    // Tangents from S and T to each circle
    for (int i = 0; i < N; ++i){
        addTangentsFromPoint(S, idS, i);
        addTangentsFromPoint(Tt, idT, i);
    }

    // Bitangents between circles
    for (int i = 0; i < N; ++i){
        for (int j = i+1; j < N; ++j){
            addCirclePairBitangents(i, j);
        }
    }

    // Also, S <-> T edge if safe (again; might be added above but ensure)
    addEdgeIds(idS, idT);

    // Dijkstra
    int M = (int)nodes.size();
    const double INF = 1e100;
    vector<double> dist(M, INF);
    vector<int> par(M, -1);
    vector<char> used(M, 0);
    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;
    dist[idS] = 0.0;
    pq.push({0.0, idS});
    while (!pq.empty()){
        auto [dcur, u] = pq.top(); pq.pop();
        if (used[u]) continue;
        used[u] = 1;
        if (u == idT) break;
        for (auto &e : adj[u]){
            int v = e.first;
            double w = e.second;
            if (dist[v] > dcur + w){
                dist[v] = dcur + w;
                par[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Reconstruct path
    vector<int> path;
    int cur = idT;
    if (par[cur] == -1 && cur != idS){
        // As a last resort, output no intermediate points; the direct edge was not free so this should be rare,
        // but the constructed graph should be connected in valid instances.
        // Still, guard by printing S->T with one corner waypoint if available and safe.
        // Try via any corner
        bool ok = false;
        for (int k = 0; k < 4 && !ok; ++k){
            if (isFreeSegment(S, corners[k]) && isFreeSegment(corners[k], Tt)){
                cout << 1 << "\n";
                cout.setf(std::ios::fixed); cout << setprecision(12);
                cout << corners[k].x << " " << corners[k].y << "\n";
                return 0;
            }
        }
        // Fallback to zero (though judge guarantees existence; this should not happen)
        cout << 0 << "\n";
        return 0;
    }
    while (cur != -1){
        path.push_back(cur);
        cur = par[cur];
    }
    reverse(path.begin(), path.end());

    // Extract intermediate points, remove consecutive duplicates
    vector<Pt> P;
    for (int i = 1; i+1 < (int)path.size(); ++i){
        Pt p = nodes[path[i]];
        if (!P.empty()){
            Pt d = p - P.back();
            if (norm2(d) <= 1e-16) continue;
        }
        P.push_back(p);
    }

    cout << P.size() << "\n";
    cout.setf(std::ios::fixed);
    cout << setprecision(12);
    for (auto &p : P){
        cout << p.x << " " << p.y << "\n";
    }
    return 0;
}
```