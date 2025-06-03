#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <cstdio>

using namespace std;

const double EPS = 1e-9;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return fabs(x - other.x) < EPS && fabs(y - other.y) < EPS;
    }
};

struct PointComparator {
    bool operator()(const Point& a, const Point& b) const {
        if (fabs(a.x - b.x) > EPS) return a.x < b.x;
        if (fabs(a.y - b.y) > EPS) return a.y < b.y;
        return false;
    }
};

double cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

bool onSegment(const Point& a, const Point& b, const Point& p) {
    if (fabs(cross(a, b, p)) > EPS)
        return false;
    double minX = min(a.x, b.x);
    double maxX = max(a.x, b.x);
    double minY = min(a.y, b.y);
    double maxY = max(a.y, b.y);
    return (minX - EPS <= p.x && p.x <= maxX + EPS) && 
           (minY - EPS <= p.y && p.y <= maxY + EPS);
}

vector<Point> convex_hull(vector<Point> P) {
    int n = P.size();
    if (n <= 1) return P;

    sort(P.begin(), P.end(), [](const Point& a, const Point& b) {
        return a.x < b.x - EPS || (fabs(a.x - b.x) < EPS && a.y < b.y - EPS;
    });

    vector<Point> H;
    H.resize(2 * n);
    int k = 0;

    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= EPS) 
            k--;
        H[k++] = P[i];
    }

    for (int i = n-2, t = k+1; i >= 0; --i) {
        while (k >= t && cross(H[k-2], H[k-1], P[i]) <= EPS) 
            k--;
        H[k++] = P[i];
    }

    H.resize(k - 1);
    return H;
}

double compute_area(const vector<Point>& P) {
    double area = 0.0;
    int n = P.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += P[i].x * P[j].y - P[j].x * P[i].y;
    }
    return fabs(area) / 2.0;
}

int main() {
    int n;
    cin >> n;
    vector<Point> P(n);
    for (int i = 0; i < n; i++) {
        cin >> P[i].x >> P[i].y;
    }

    if (n % 2 == 0) {
        int half = n / 2;
        Point center((P[0].x + P[half].x) / 2.0, (P[0].y + P[half].y) / 2.0);
        bool symmetric = true;
        for (int i = 0; i < half; i++) {
            Point sym(2 * center.x - P[i].x, 2 * center.y - P[i].y);
            int j = i + half;
            if (fabs(sym.x - P[j].x) > EPS || fabs(sym.y - P[j].y) > EPS) {
                symmetric = false;
                break;
            }
        }
        if (symmetric) {
            double area = compute_area(P);
            printf("%.15f\n", area);
            return 0;
        }
    }

    set<Point, PointComparator> candidate_set;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Point c((P[i].x + P[j].x) / 2.0, (P[i].y + P[j].y) / 2.0);
            candidate_set.insert(c);
        }
    }

    vector<tuple<double, double, double>> events;

    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double dx = P[j].x - P[i].x;
        double dy = P[j].y - P[i].y;
        for (int k = 0; k < n; k++) {
            double a = -2 * dy;
            double b = 2 * dx;
            double c_val = dx * (P[i].y + P[k].y) - dy * (P[i].x + P[k].x);
            events.push_back(make_tuple(a, b, c_val));
        }
    }

    for (int j = 0; j < n; j++) {
        int k = (j + 1) % n;
        double a = 2 * (P[j].y - P[k].y);
        double b = 2 * (P[k].x - P[j].x);
        for (int i = 0; i < n; i++) {
            double c_val = (P[j].y - P[k].y) * (P[i].x + P[j].x) + (P[k].x - P[j].x) * (P[i].y + P[j].y);
            events.push_back(make_tuple(a, b, c_val));
        }
    }

    for (int i = 0; i < events.size(); i++) {
        for (int j = i + 1; j < events.size(); j++) {
            double a1 = get<0>(events[i]);
            double b1 = get<1>(events[i]);
            double c1 = get<2>(events[i]);
            double a2 = get<0>(events[j]);
            double b2 = get<1>(events[j]);
            double c2 = get<2>(events[j]);

            double det = a1 * b2 - a2 * b1;
            if (fabs(det) < EPS) 
                continue;

            double cx = (c1 * b2 - c2 * b1) / det;
            double cy = (a1 * c2 - a2 * c1) / det;
            candidate_set.insert(Point(cx, cy));
        }
    }

    double min_area = 1e300;
    bool found = false;

    for (const Point& c : candidate_set) {
        set<Point, PointComparator> S_set;
        for (int i = 0; i < n; i++) {
            S_set.insert(P[i]);
            Point sym(2 * c.x - P[i].x, 2 * c.y - P[i].y);
            S_set.insert(sym);
        }

        vector<Point> S_unique;
        for (const Point& p : S_set) {
            S_unique.push_back(p);
        }

        if (S_unique.size() < 3) 
            continue;

        vector<Point> hull = convex_hull(S_unique);
        bool valid = true;
        for (int i = 0; i < n; i++) {
            bool on_boundary = false;
            for (int j = 0; j < hull.size(); j++) {
                if (fabs(hull[j].x - P[i].x) < EPS && fabs(hull[j].y - P[i].y) < EPS) {
                    on_boundary = true;
                    break;
                }
            }
            if (on_boundary) continue;
            for (int j = 0; j < hull.size(); j++) {
                int k = (j + 1) % hull.size();
                if (onSegment(hull[j], hull[k], P[i])) {
                    on_boundary = true;
                    break;
                }
            }
            if (!on_boundary) {
                valid = false;
                break;
            }
        }

        if (!valid) 
            continue;

        double area = compute_area(hull);
        if (area < min_area) {
            min_area = area;
            found = true;
        }
    }

    if (found) {
        printf("%.15f\n", min_area);
    } else {
        printf("-1\n");
    }

    return 0;
}