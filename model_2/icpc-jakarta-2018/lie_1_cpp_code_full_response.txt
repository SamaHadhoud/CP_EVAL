```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    int cntLie = 0;
    string s;
    for (int i = 0; i < N; ++i) {
        cin >> s;
        if (s == "LIE") ++cntLie;
    }
    cout << (cntLie % 2 == 0 ? "TRUTH" : "LIE") << '\n';
    return 0;
}
```