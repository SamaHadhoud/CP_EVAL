#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <climits>

using namespace std;

int to_minutes(const string &time_str) {
    int hours = stoi(time_str.substr(0, 2));
    int minutes = stoi(time_str.substr(3, 2));
    return hours * 60 + minutes;
}

string to_time(int minutes) {
    int h = minutes / 60;
    int m = minutes % 60;
    char buffer[6];
    sprintf(buffer, "%02d:%02d", h, m);
    return string(buffer);
}

int main() {
    int n;
    cin >> n;
    cin.ignore();

    vector<string> lines;
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        lines.push_back(line);
    }

    vector<int> s_list, e_list;
    for (int i = 0; i < n; i++) {
        string line = lines[i];
        size_t pos = line.find('-');
        string start_str = line.substr(0, pos - 1);
        start_str.erase(remove(start_str.begin(), start_str.end(), ' '), start_str.end());
        string end_str = line.substr(pos + 1);
        end_str.erase(remove(end_str.begin(), end_str.end(), ' '), end_str.end());
        s_list.push_back(to_minutes(start_str));
        e_list.push_back(to_minutes(end_str));
    }

    int first_start = s_list[0];
    int last_end = e_list[n - 1];
    int total_minutes = last_end - first_start;

    auto feasible = [&](int L) -> bool {
        vector<int> A(n, 0);
        vector<int> B(n, 0);

        B[n - 1] = last_end;
        A[n - 1] = min(s_list[n - 1], last_end - L);
        A[n - 1] = max(16 * 60, A[n - 1]);

        for (int i = n - 2; i >= 1; i--) {
            B[i] = A[i + 1];
            A[i] = min(s_list[i], B[i] - L);
            A[i] = max(16 * 60, A[i]);
        }

        if (n >= 2) {
            B[0] = A[1];
        }
        A[0] = min(s_list[0], B[0] - L);

        if (A[0] != first_start) {
            return false;
        }

        for (int i = 0; i < n; i++) {
            if (B[i] < e_list[i]) {
                return false;
            }
            if (B[i] - A[i] < L) {
                return false;
            }
        }
        return true;
    };

    int low = 0;
    int high = total_minutes;
    int best_L = 0;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (feasible(mid)) {
            best_L = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    vector<int> A(n, 0);
    vector<int> B(n, 0);

    B[n - 1] = last_end;
    A[n - 1] = min(s_list[n - 1], last_end - best_L);
    A[n - 1] = max(16 * 60, A[n - 1]);

    for (int i = n - 2; i >= 1; i--) {
        B[i] = A[i + 1];
        A[i] = min(s_list[i], B[i] - best_L);
        A[i] = max(16 * 60, A[i]);
    }

    if (n >= 2) {
        B[0] = A[1];
    }
    A[0] = min(s_list[0], B[0] - best_L);

    for (int i = 0; i < n; i++) {
        cout << to_time(A[i]) << " - " << to_time(B[i]) << endl;
    }

    return 0;
}