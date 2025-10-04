```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    if (!(cin >> N)) return 0;
    for (int i = 0; i < N; ++i) {
        int x; 
        cin >> x;
        int b = N + 1 - x;
        if (i) cout << ' ';
        cout << b;
    }
    cout << '\n';
    return 0;
}
```