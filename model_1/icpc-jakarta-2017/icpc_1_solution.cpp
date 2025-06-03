#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;

    vector<int> T(M);
    for (int j = 0; j < M; j++) {
        cin >> T[j];
    }

    int best_count = -1;
    int best_team_index = -1;

    for (int i = 0; i < N; i++) {
        vector<int> S(M);
        for (int j = 0; j < M; j++) {
            cin >> S[j];
        }

        int count = 0;
        for (int j = 0; j < M; j++) {
            if (S[j] == T[j]) {
                count++;
            }
        }

        if (count > best_count) {
            best_count = count;
            best_team_index = i;
        }
    }

    cout << best_team_index + 1 << endl;

    return 0;
}