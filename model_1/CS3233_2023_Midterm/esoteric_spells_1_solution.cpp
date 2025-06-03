#include <iostream>
#include <vector>
using namespace std;

int main() {
    int C;
    cin >> C;
    while (C--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        long long T = 0;
        for (long long num : a) {
            T |= num;
        }

        vector<long long> x(n, 0);

        for (int j = 0; j <= 60; j++) {
            long long mask = 1LL << j;
            if (T & mask) {
                vector<int> indices;
                for (int i = 0; i < n; i++) {
                    if (a[i] & mask) {
                        indices.push_back(i);
                    }
                }
                if (indices.size() % 2 == 0) {
                    if (!indices.empty()) {
                        indices.pop_back();
                    }
                }
                for (int idx : indices) {
                    x[idx] |= mask;
                }
            }
        }

        cout << T << endl;
        for (int i = 0; i < n; i++) {
            cout << x[i];
            if (i < n-1) {
                cout << " ";
            }
        }
        cout << endl;
    }
    return 0;
}