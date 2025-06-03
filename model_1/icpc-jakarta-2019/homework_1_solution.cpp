#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n;
    cin >> n;
    vector<int> A(n);
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }

    if (n % 2 == 0) {
        for (int i = n - 1; i >= 0; i--) {
            cout << A[i];
            if (i > 0) {
                cout << " ";
            }
        }
    } else {
        for (int i = 1; i < n; i++) {
            cout << A[i] << " ";
        }
        cout << A[0];
    }
    cout << endl;
    
    return 0;
}