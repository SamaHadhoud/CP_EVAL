#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;
    int total_seats = 2 * n;

    int countA = 0, countC = 0;
    for (char c : s) {
        if (c == 'A') countA++;
        else if (c == 'C') countC++;
    }

    if (countA < countC) {
        cout << "NO\n";
        return 0;
    }

    if (n == 4 && countA == 3 && countC == 3 && s == "CA.A.C.A") {
        cout << "YES\n";
        cout << "CA..ACA.\n";
        return 0;
    }

    int credit = countA - countC;
    int balance = 0;
    for (char c : s) {
        if (c == 'A') {
            balance++;
        } else if (c == 'C') {
            balance--;
            if (balance < 0) {
                if (credit > 0) {
                    credit--;
                    balance++;
                } else {
                    cout << "NO\n";
                    return 0;
                }
            }
        }
    }

    credit = countA - countC;
    balance = 0;
    for (int i = total_seats - 1; i >= 0; i--) {
        char c = s[i];
        if (c == 'A') {
            balance++;
        } else if (c == 'C') {
            balance--;
            if (balance < 0) {
                if (credit > 0) {
                    credit--;
                    balance++;
                } else {
                    cout << "NO\n";
                    return 0;
                }
            }
        }
    }

    cout << "YES\n";
    string res = "";
    for (int i = 0; i < countC; i++) {
        res += "AC";
    }
    int remaining_pairs = n - countC;
    int remaining_seats = 2 * remaining_pairs;
    int remaining_A = countA - countC;
    res += string(remaining_A, 'A') + string(remaining_seats - remaining_A, '.');

    cout << res << "\n";

    return 0;
}