```cpp
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Point {
    ll x, y;
};

static inline bool onBorder(ll x, ll y, ll X1, ll X2, ll Y1, ll Y2) {
    return (((x == X1) || (x == X2)) && (y >= Y1 && y <= Y2)) ||
           (((y == Y1) || (y == Y2)) && (x >= X1 && x <= X2));
}

bool validSquare(const vector<Point>& P, ll X1, ll X2, ll Y1, ll Y2) {
    if (X1 > X2 || Y1 > Y2) return false;
    if ((X2 - X1) != (Y2 - Y1)) return false;
    for (const auto& pt : P) {
        if (!onBorder(pt.x, pt.y, X1, X2, Y1, Y2)) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    vector<Point> P;
    P.reserve(N);
    for (int i = 0; i < N; ++i) {
        ll x, y;
        cin >> x >> y;
        P.push_back({x, y});
    }

    ll minX = P[0].x, maxX = P[0].x, minY = P[0].y, maxY = P[0].y;
    for (const auto& pt : P) {
        minX = min(minX, pt.x);
        maxX = max(maxX, pt.x);
        minY = min(minY, pt.y);
        maxY = max(maxY, pt.y);
    }

    ll dx = maxX - minX;
    ll dy = maxY - minY;
    ll L = max(dx, dy);

    // Case 1: bounding rectangle already a square
    if (dx == dy) {
        if (validSquare(P, minX, maxX, minY, maxY)) {
            cout << minX << ' ' << maxX << ' ' << minY << ' ' << maxY << '\n';
        } else {
            cout << "Impossible\n";
        }
        return 0;
    }

    // Case 2: taller than wide (dx < dy): try keeping left or right side
    if (dx < dy) {
        ll X1, X2, Y1 = minY, Y2 = maxY;

        // Candidate A: keep left side at minX
        X1 = minX; X2 = minX + L;
        if (validSquare(P, X1, X2, Y1, Y2)) {
            cout << X1 << ' ' << X2 << ' ' << Y1 << ' ' << Y2 << '\n';
            return 0;
        }

        // Candidate B: keep right side at maxX
        X1 = maxX - L; X2 = maxX;
        if (validSquare(P, X1, X2, Y1, Y2)) {
            cout << X1 << ' ' << X2 << ' ' << Y1 << ' ' << Y2 << '\n';
            return 0;
        }

        cout << "Impossible\n";
        return 0;
    }

    // Case 3: wider than tall (dx > dy): try keeping bottom or top side
    {
        ll X1 = minX, X2 = maxX, Y1, Y2;

        // Candidate A: keep bottom at minY
        Y1 = minY; Y2 = minY + L;
        if (validSquare(P, X1, X2, Y1, Y2)) {
            cout << X1 << ' ' << X2 << ' ' << Y1 << ' ' << Y2 << '\n';
            return 0;
        }

        // Candidate B: keep top at maxY
        Y1 = maxY - L; Y2 = maxY;
        if (validSquare(P, X1, X2, Y1, Y2)) {
            cout << X1 << ' ' << X2 << ' ' << Y1 << ' ' << Y2 << '\n';
            return 0;
        }

        cout << "Impossible\n";
        return 0;
    }
}
```