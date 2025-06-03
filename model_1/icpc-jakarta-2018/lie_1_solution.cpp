#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int N;
    cin >> N;
    cin.ignore();

    vector<string> reports(N);
    for (int i = 0; i < N; i++) {
        getline(cin, reports[i]);
    }

    bool current = true;
    for (int i = N - 2; i >= 0; i--) {
        bool report_value = (reports[i+1] == "TRUTH");
        current = (current == report_value);
    }

    bool first_report = (reports[0] == "TRUTH");
    bool testimony = (current == first_report);

    if (testimony) {
        cout << "TRUTH" << endl;
    } else {
        cout << "LIE" << endl;
    }

    return 0;
}