#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int N;
    cin >> N;
    
    vector<pair<int, int>> tree1;
    tree1.reserve(N - 1);
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        tree1.push_back({u, v});
    }
    
    unordered_set<long long> tree2Set;
    tree2Set.reserve(N - 1);
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        if (u > v) swap(u, v);
        long long key = 1LL * u * (N + 1) + v;
        tree2Set.insert(key);
    }
    
    int count = 0;
    for (auto edge : tree1) {
        long long key = 1LL * edge.first * (N + 1) + edge.second;
        if (tree2Set.find(key) == tree2Set.end()) {
            count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}