#include <bits/stdc++.h>
using namespace std;

using ld = long double;

const ld EPS = 1e-12;
const ld COL_EPS = 1e-10;
const ld BND_EPS = 1e-10;

struct Point {
    ld x, y;
    Point() {}
    Point(ld _x, ld _y): x(_x), y(_y) {}
    Point operator + (const Point& o) const { return Point(x + o.x, y + o.y); }
    Point operator - (const Point& o) const { return Point(x - o.x, y - o.y); }
    Point operator * (ld k) const { return Point(x * k, y * k); }
    Point operator / (ld k) const { return Point(x / k, y / k); }
};

static inline ld dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
static inline ld cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
static inline ld cross(const Point& a, const Point& b, const Point& c) { return cross(b - a, c - a); }

bool eq(const Point& a, const Point& b) {
    return fabsl(a.x - b.x) <= BND_EPS && fabsl(a.y - b.y) <= BND_EPS;
}

bool on_segment(const Point& a, const Point& b, const Point& p) {
    if (fabsl(cross(a, b, p)) > COL_EPS) return false;
    if (p.x < min(a.x, b.x) - BND_EPS || p.x > max(a.x, b.x) + BND_EPS) return false;
    if (p.y < min(a.y, b.y) - BND_EPS || p.y > max(a.y, b.y) + BND_EPS) return false;
    return true;
}

vector<Point> convex_hull(vector<Point> pts) {
    int m = (int)pts.size();
    if (m <= 1) return pts;
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b){
        if (a.x < b.x) return true;
        if (a.x > b.x) return false;
        return a.y < b.y;
    });
    vector<Point> lower, upper;
    for (int i = 0; i < m; ++i) {
        while ((int)lower.size() >= 2) {
            Point a = lower[lower.size()-2], b = lower[lower.size()-1], c = pts[i];
            if (cross(a, b, c) <= EPS) lower.pop_back();
            else break;
        }
        lower.push_back(pts[i]);
    }
    for (int i = m-1; i >= 0; --i) {
        while ((int)upper.size() >= 2) {
            Point a = upper[upper.size()-2], b = upper[upper.size()-1], c = pts[i];
            if (cross(a, b, c) <= EPS) upper.pop_back();
            else break;
        }
        upper.push_back(pts[i]);
    }
    // Concatenate lower and upper, removing last point of each (duplicate of first of the other)
    lower.pop_back();
    upper.pop_back();
    vector<Point> hull = lower;
    hull.insert(hull.end(), upper.begin(), upper.end());
    return hull;
}

bool point_on_boundary_of_hull(const Point& p, const vector<Point>& H) {
    int m = (int)H.size();
    for (int i = 0; i < m; ++i) {
        if (eq(p, H[i])) return true;
    }
    for (int i = 0; i < m; ++i) {
        const Point& a = H[i];
        const Point& b = H[(i+1)%m];
        if (on_segment(a, b, p)) return true;
    }
    return false;
}

ld polygon_area(const vector<Point>& poly) {
    int m = (int)poly.size();
    ld s = 0;
    for (int i = 0; i < m; ++i) {
        const Point& a = poly[i];
        const Point& b = poly[(i+1)%m];
        s += cross(a, b);
    }
    return fabsl(s) * 0.5L;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<Point> P(n);
    for (int i = 0; i < n; ++i) {
        long long xi, yi;
        cin >> xi >> yi;
        P[i] = Point((ld)xi, (ld)yi);
    }

    // Precompute normals and balancing lines
    struct LineN {
        Point n; // normal vector
        ld a;    // dot(n, C) = a
    };
    vector<LineN> lines;
    lines.reserve(n);
    for (int j = 0; j < n; ++j) {
        Point pj = P[j];
        Point pj1 = P[(j+1)%n];
        Point e = pj1 - pj;
        Point nj(e.y, -e.x); // outward normal for CCW polygon
        ld bj = dot(nj, pj); // support value at edge
        ld mj = bj;
        // find min over all vertices
        for (int i = 0; i < n; ++i) {
            ld val = dot(nj, P[i]);
            if (i == 0 || val < mj) mj = val;
        }
        ld aj = (bj + mj) / 2.0L;
        lines.push_back({nj, aj});
    }

    // Build candidate centers: intersections of balancing lines, and all vertex midpoints
    vector<Point> candidates;
    candidates.reserve(n*n + n*(n-1)/2);
    for (int j = 0; j < n; ++j) {
        for (int k = j+1; k < n; ++k) {
            Point n1 = lines[j].n, n2 = lines[k].n;
            ld a1 = lines[j].a, a2 = lines[k].a;
            ld det = n1.x * n2.y - n1.y * n2.x;
            if (fabsl(det) < EPS) continue; // parallel
            // Solve:
            // n1.x * cx + n1.y * cy = a1
            // n2.x * cx + n2.y * cy = a2
            ld cx = (a1 * n2.y - n1.y * a2) / det;
            ld cy = (n1.x * a2 - a1 * n2.x) / det;
            candidates.emplace_back(cx, cy);
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int k = i+1; k < n; ++k) {
            Point c = (P[i] + P[k]) / 2.0L;
            candidates.push_back(c);
        }
    }

    ld best = numeric_limits<ld>::infinity();
    bool found = false;

    vector<Point> allPts;
    allPts.reserve(2*n);

    for (const Point& C : candidates) {
        allPts.clear();
        // P union Q with Q = 2C - P
        for (int i = 0; i < n; ++i) allPts.push_back(P[i]);
        for (int i = 0; i < n; ++i) {
            Point q = C * 2.0L - P[i];
            allPts.push_back(q);
        }
        vector<Point> H = convex_hull(allPts);

        bool ok = true;
        for (int i = 0; i < n && ok; ++i) {
            if (!point_on_boundary_of_hull(P[i], H)) ok = false;
        }
        if (!ok) continue;

        ld area = polygon_area(H);
        if (area < best) {
            best = area;
            found = true;
        }
    }

    cout.setf(std::ios::fixed);
    cout << setprecision(12);
    if (!found) {
        cout << -1 << "\n";
    } else {
        cout << (double)best << "\n";
    }

    return 0;
}