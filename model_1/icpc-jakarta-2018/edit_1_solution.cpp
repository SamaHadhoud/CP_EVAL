#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cin >> s;
    string t = "";
    for (char c : s) {
        if (c == '0') {
            t += '1';
        } else {
            t += '0';
        }
    }
    cout << t << endl;
    return 0;
}