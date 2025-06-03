#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    int N, M;
    std::cin >> N >> M;

    std::vector<std::tuple<int, long long, long long>> edges;
    for (int i = 0; i < N; i++) {
        long long u, v;
        int w;
        std::cin >> u >> v >> w;
        edges.push_back(std::make_tuple(w, u, v));
    }

    std::sort(edges.begin(), edges.end());

    int trees = 0;
    std::unordered_map<long long, long long> slots;
    slots.reserve(2 * N);

    for (int i = 0; i < N; i++) {
        int w = std::get<0>(edges[i]);
        long long u = std::get<1>(edges[i]);
        long long v = std::get<2>(edges[i]);

        long long& ref_u = slots[u];
        long long& ref_v = slots[v];

        if (ref_u > 0) {
            ref_u--;
            ref_v += M;
        } else {
            trees++;
            ref_u += (M - 1);
            ref_v += M;
        }
    }

    std::cout << trees << "\n";
    return 0;
}