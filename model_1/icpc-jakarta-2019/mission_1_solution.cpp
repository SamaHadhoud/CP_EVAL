#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <queue>
using namespace std;

const double epsilon = 1e-6;
const double PI = acos(-1.0);

struct Point {
    double x, y;
    Point(double x=0, double y=0): x(x), y(y) {}
    Point operator-(const Point& b) const {
        return Point(x - b.x, y - b.y);
    }
    Point operator+(const Point& b) const {
        return Point(x + b.x, y + b.y);
    }
    Point operator*(double s) const {
        return Point(x * s, y * s);
    }
    double dot(const Point& b) const {
        return x * b.x + y * b.y;
    }
    double norm_sq() const {
        return x*x + y*y;
    }
    double norm() const {
        return sqrt(norm_sq());
    }
    Point rotate(double angle) const {
        double c = cos(angle), s = sin(angle);
        return Point(x*c - y*s, x*s + y*c);
    }
};

bool point_equal(const Point& a, const Point& b) {
    return fabs(a.x - b.x) <= epsilon && fabs(a.y - b.y) <= epsilon;
}

struct Circle {
    Point c;
    double r;
    Circle(Point c, double r): c(c), r(r) {}
};

vector<Point> tangent_points_from_point(Point p, Circle circle) {
    vector<Point> res;
    Point d_vec = circle.c - p;
    double d_sq = d_vec.norm_sq();
    if (d_sq < circle.r*circle.r - epsilon) {
        return res;
    }
    if (fabs(d_sq - circle.r*circle.r) < epsilon) {
        res.push_back(p);
        return res;
    }
    double d = sqrt(d_sq);
    double angle = atan2(d_vec.y, d_vec.x);
    double da = acos(circle.r / d);
    Point p1(
        circle.c.x + circle.r * cos(angle - da),
        circle.c.y + circle.r * sin(angle - da)
    );
    Point p2(
        circle.c.x + circle.r * cos(angle + da),
        circle.c.y + circle.r * sin(angle + da)
    );
    res.push_back(p1);
    res.push_back(p2);
    return res;
}

vector<pair<Point, Point>> common_tangents(Circle c1, Circle c2) {
    vector<pair<Point, Point>> tangents;
    double d_sq = (c1.c - c2.c).norm_sq();
    if (d_sq < epsilon) {
        return tangents;
    }
    double d = sqrt(d_sq);
    Point v = (c2.c - c1.c) * (1.0/d);
    for (int sign = 1; sign >= -1; sign -= 2) {
        double r_diff = sign * c1.r - sign * c2.r;
        if (fabs(r_diff) < epsilon) {
            Point a = v.rotate(PI/2);
            Point offset1 = a * c1.r;
            tangents.push_back({c1.c + offset1, c2.c + offset1});
            tangents.push_back({c1.c + (offset1*(-1)), c2.c + (offset1*(-1))});
        } else {
            double ratio = (c1.r - sign * c2.r) / d;
            if (fabs(ratio) > 1.0) {
                continue;
            }
            double angle = acos(ratio);
            Point a1 = v.rotate(angle);
            Point a2 = v.rotate(-angle);
            Point offset1 = a1 * c1.r;
            Point offset2 = a2 * c1.r;
            Point dir_vec = v * (sign * c1.r);
            Point point1_1 = c1.c + offset1;
            Point point1_2 = c1.c + offset2;
            Point point2_1 = c2.c + dir_vec + offset1.rotate(PI);
            Point point2_2 = c2.c + dir_vec + offset2.rotate(PI);
            tangents.push_back({point1_1, point2_1});
            tangents.push_back({point1_2, point2_2});
        }
    }
    for (int sign = 1; sign >= -1; sign -= 2) {
        double r_sum = c1.r + sign * c2.r;
        double ratio = r_sum / d;
        if (ratio > 1.0) {
            continue;
        }
        double angle = acos(ratio);
        Point a1 = v.rotate(angle);
        Point a2 = v.rotate(-angle);
        Point offset1 = a1 * c1.r;
        Point offset2 = a2 * c1.r;
        Point point1_1 = c1.c + offset1;
        Point point1_2 = c1.c + offset2;
        Point dir_vec = v * (-sign * c2.r);
        Point point2_1 = c2.c + offset1.rotate(PI) + dir_vec;
        Point point2_2 = c2.c + offset2.rotate(PI) + dir_vec;
        tangents.push_back({point1_1, point2_1});
        tangents.push_back({point1_2, point2_2});
    }
    return tangents;
}

double point_to_segment_distance_sq(const Point& p, const Point& a, const Point& b) {
    if (point_equal(a, b)) {
        return (p - a).norm_sq();
    }
    Point ab = b - a;
    Point ap = p - a;
    double ab2 = ab.norm_sq();
    double t = ap.dot(ab) / ab2;
    t = max(0.0, min(1.0, t));
    Point projection = a + ab * t;
    return (p - projection).norm_sq();
}

bool segment_safe(const Point& a, const Point& b, const vector<Circle>& circles) {
    for (const Circle& circle : circles) {
        double d_sq = point_to_segment_distance_sq(circle.c, a, b);
        double r_minus = circle.r - epsilon;
        if (r_minus < 0) r_minus = 0;
        if (d_sq < r_minus * r_minus) {
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    double xL, yL, xR, yR;
    cin >> n >> xL >> yL >> xR >> yR;
    double xs, ys, xt, yt;
    cin >> xs >> ys >> xt >> yt;

    vector<Circle> circles;
    for (int i=0; i<n; i++) {
        double x, y, r;
        cin >> x >> y >> r;
        circles.push_back(Circle(Point(x,y), r));
    }

    vector<Point> points;
    points.push_back(Point(xs, ys));
    points.push_back(Point(xt, yt));

    vector<Point> corners;
    corners.push_back(Point(xL, yL));
    corners.push_back(Point(xL, yR));
    corners.push_back(Point(xR, yL));
    corners.push_back(Point(xR, yR));
    for (const Point& p : corners) {
        points.push_back(p);
    }

    for (const Circle& circle : circles) {
        vector<Point> tangents = tangent_points_from_point(Point(xs, ys), circle);
        for (const Point& p : tangents) {
            if (p.x < xL - epsilon || p.x > xR + epsilon || p.y < yL - epsilon || p.y > yR + epsilon) {
                continue;
            }
            points.push_back(p);
        }
    }

    for (const Circle& circle : circles) {
        vector<Point> tangents = tangent_points_from_point(Point(xt, yt), circle);
        for (const Point& p : tangents) {
            if (p.x < xL - epsilon || p.x > xR + epsilon || p.y < yL - epsilon || p.y > yR + epsilon) {
                continue;
            }
            points.push_back(p);
        }
    }

    for (int i=0; i<circles.size(); i++) {
        for (int j=i+1; j<circles.size(); j++) {
            vector<pair<Point, Point>> tangents = common_tangents(circles[i], circles[j]);
            for (const auto& tangent : tangents) {
                Point p1 = tangent.first;
                Point p2 = tangent.second;
                if (p1.x >= xL - epsilon && p1.x <= xR + epsilon && p1.y >= yL - epsilon && p1.y <= yR + epsilon) {
                    points.push_back(p1);
                }
                if (p2.x >= xL - epsilon && p2.x <= xR + epsilon && p2.y >= yL - epsilon && p2.y <= yR + epsilon) {
                    points.push_back(p2);
                }
            }
        }
    }

    vector<Point> unique_points;
    for (const Point& p : points) {
        bool found = false;
        for (const Point& q : unique_points) {
            if (fabs(p.x - q.x) <= epsilon && fabs(p.y - q.y) <= epsilon) {
                found = true;
                break;
            }
        }
        if (!found) {
            unique_points.push_back(p);
        }
    }
    points = unique_points;

    int n_points = points.size();
    vector<vector<int>> graph(n_points);
    for (int i=0; i<n_points; i++) {
        for (int j=i+1; j<n_points; j++) {
            if (segment_safe(points[i], points[j], circles)) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }

    vector<int> parent(n_points, -1);
    queue<int> q;
    parent[0] = 0;
    q.push(0);
    bool found_path = false;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (u == 1) {
            found_path = true;
            break;
        }
        for (int v : graph[u]) {
            if (parent[v] == -1) {
                parent[v] = u;
                q.push(v);
            }
        }
    }

    if (!found_path) {
        if (segment_safe(points[0], points[1], circles)) {
            cout << 0 << endl;
            return 0;
        }
        cout << 0 << endl;
        return 0;
    }

    vector<int> path;
    int node = 1;
    while (node != 0) {
        path.push_back(node);
        node = parent[node];
    }
    path.push_back(0);
    reverse(path.begin(), path.end());

    cout << path.size() - 2 << endl;
    for (int i = 1; i < path.size()-1; i++) {
        Point p = points[path[i]];
        cout << fixed << setprecision(10) << p.x << " " << p.y << endl;
    }

    return 0;
}