#include <iostream>
#include <vector>
#include <cstdint>
using namespace std;

struct Artillery {
    int x, y;
    int8_t count;
    int walls_below[5];
};

struct Wall {
    int x1, x2, y0;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, Q;
    cin >> N >> M >> Q;

    vector<Artillery> artilleries(N);
    for (int i = 0; i < N; i++) {
        cin >> artilleries[i].x >> artilleries[i].y;
        artilleries[i].count = 0;
    }

    vector<Wall> walls_list(M);
    for (int j = 0; j < M; j++) {
        cin >> walls_list[j].x1 >> walls_list[j].x2 >> walls_list[j].y0;
    }

    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
            if (artilleries[i].y > walls_list[j].y0) {
                artilleries[i].walls_below[artilleries[i].count] = j;
                artilleries[i].count++;
            }
        }
    }

    for (int q = 0; q < Q; q++) {
        int x_t, y_t;
        cin >> x_t >> y_t;

        int count_visible = 0;
        for (int i = 0; i < N; i++) {
            bool blocked = false;
            int x_i = artilleries[i].x;
            int y_i = artilleries[i].y;
            for (int k = 0; k < artilleries[i].count; k++) {
                int wall_idx = artilleries[i].walls_below[k];
                const Wall& w = walls_list[wall_idx];
                int64_t base = (int64_t)(y_i - y_t);
                int64_t num = (int64_t)x_t * (y_i - w.y0) + (int64_t)x_i * (w.y0 - y_t);
                int64_t L_bound = (int64_t)w.x1 * base;
                int64_t R_bound = (int64_t)w.x2 * base;
                if (num >= L_bound && num <= R_bound) {
                    blocked = true;
                    break;
                }
            }
            if (!blocked) {
                count_visible++;
            }
        }
        cout << count_visible << '\n';
    }

    return 0;
}