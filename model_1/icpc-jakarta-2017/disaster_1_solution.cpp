#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    long long x1, y1, x2, y2, N;
    cin >> x1 >> y1 >> x2 >> y2 >> N;

    vector<vector<long long>> circles;
    for (long long i = 0; i < N; i++) {
        long long fx, fy, r;
        cin >> fx >> fy >> r;
        circles.push_back({fx, fy, r});
    }

    vector<vector<long long>> kept_circles;
    for (auto &circle : circles) {
        long long fx = circle[0];
        long long fy = circle[1];
        long long r = circle[2];
        long long dx = 0;
        long long dy = 0;
        if (fx < x1) {
            dx = x1 - fx;
        } else if (fx > x2) {
            dx = fx - x2;
        }
        if (fy < y1) {
            dy = y1 - fy;
        } else if (fy > y2) {
            dy = fy - y2;
        }
        long long d_sq = dx*dx + dy*dy;
        if (d_sq < r*r) {
            kept_circles.push_back({fx, fy, r});
        }
    }

    long long n = kept_circles.size();
    vector<vector<long long>> graph(n + 4);

    for (long long i = 0; i < n; i++) {
        long long fx = kept_circles[i][0];
        long long fy = kept_circles[i][1];
        long long r = kept_circles[i][2];

        long long d_sq;
        if (fy >= y1 && fy <= y2) {
            d_sq = (fx - x1) * (fx - x1);
        } else if (fy < y1) {
            d_sq = (fx - x1)*(fx - x1) + (fy - y1)*(fy - y1);
        } else {
            d_sq = (fx - x1)*(fx - x1) + (fy - y2)*(fy - y2);
        }
        if (d_sq < r * r) {
            graph[i].push_back(n);
            graph[n].push_back(i);
        }

        if (fy >= y1 && fy <= y2) {
            d_sq = (fx - x2) * (fx - x2);
        } else if (fy < y1) {
            d_sq = (fx - x2)*(fx - x2) + (fy - y1)*(fy - y1);
        } else {
            d_sq = (fx - x2)*(fx - x2) + (fy - y2)*(fy - y2);
        }
        if (d_sq < r * r) {
            graph[i].push_back(n+1);
            graph[n+1].push_back(i);
        }

        if (fx >= x1 && fx <= x2) {
            d_sq = (fy - y1) * (fy - y1);
        } else if (fx < x1) {
            d_sq = (fx - x1)*(fx - x1) + (fy - y1)*(fy - y1);
        } else {
            d_sq = (fx - x2)*(fx - x2) + (fy - y1)*(fy - y1);
        }
        if (d_sq < r * r) {
            graph[i].push_back(n+2);
            graph[n+2].push_back(i);
        }

        if (fx >= x1 && fx <= x2) {
            d_sq = (fy - y2) * (fy - y2);
        } else if (fx < x1) {
            d_sq = (fx - x1)*(fx - x1) + (fy - y2)*(fy - y2);
        } else {
            d_sq = (fx - x2)*(fx - x2) + (fy - y2)*(fy - y2);
        }
        if (d_sq < r * r) {
            graph[i].push_back(n+3);
            graph[n+3].push_back(i);
        }
    }

    for (long long i = 0; i < n; i++) {
        for (long long j = i+1; j < n; j++) {
            long long dx = kept_circles[i][0] - kept_circles[j][0];
            long long dy = kept_circles[i][1] - kept_circles[j][1];
            long long r_sum = kept_circles[i][2] + kept_circles[j][2];
            long long d_sq = dx*dx + dy*dy;
            if (d_sq <= r_sum * r_sum) {
                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }

    vector<bool> visited(n+4, false);
    queue<long long> q;
    q.push(n);
    visited[n] = true;
    bool found = false;
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        if (u == n+1) {
            found = true;
            break;
        }
        for (long long v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    if (found) {
        cout << "NO" << endl;
        return 0;
    }

    visited.assign(n+4, false);
    q = queue<long long>();
    q.push(n+2);
    visited[n+2] = true;
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        if (u == n+3) {
            found = true;
            break;
        }
        for (long long v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    if (found) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
    }

    return 0;
}