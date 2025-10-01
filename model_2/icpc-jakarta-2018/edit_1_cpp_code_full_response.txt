#include <iostream>
#include <string>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s;
    if (!(cin >> s)) return 0;
    int n = (int)s.size();
    int c1 = 0;
    for (char ch : s) if (ch == '1') ++c1;
    int c0 = n - c1;

    if (c1 != c0) {
        char fill = (c1 > c0 ? '0' : '1');
        cout << string(n, fill) << '\n';
    } else {
        char b = s[0];
        char first = (b == '0' ? '1' : '0');
        cout << first << string(n - 1, b) << '\n';
    }
    return 0;
}