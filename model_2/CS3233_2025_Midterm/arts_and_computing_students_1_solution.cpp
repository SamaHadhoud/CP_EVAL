#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    string S;
    if (!(cin >> N)) return 0;
    cin >> S;
    int L = (int)S.size(); // should be 2N
    
    int E = 0;
    for (char c : S) if (c == '.') ++E;
    
    // Case A: No empty seats -> arrangement is fixed
    if (E == 0) {
        for (int k = 0; k < N; ++k) {
            char x = S[2*k], y = S[2*k + 1];
            if ((x == 'C' && y != 'A') || (y == 'C' && x != 'A')) {
                cout << "NO\n";
                return 0;
            }
        }
        cout << "YES\n" << S << "\n";
        return 0;
    }
    
    // Case B: At least one dot -> relative order of letters is invariant
    string T;
    T.reserve(L - E);
    for (char c : S) if (c != '.') T.push_back(c);
    int m = (int)T.size();
    
    // Greedy maximum matching on path: match adjacent unequal letters
    vector<int> mate(m, -1);
    for (int i = 0; i + 1 < m; ) {
        if (mate[i] == -1 && mate[i+1] == -1 && T[i] != T[i+1]) {
            mate[i] = i + 1;
            mate[i+1] = i;
            i += 2;
        } else {
            ++i;
        }
    }
    
    // Check every C is matched
    for (int i = 0; i < m; ++i) {
        if (T[i] == 'C' && mate[i] == -1) {
            cout << "NO\n";
            return 0;
        }
    }
    
    // Build final arrangement as pairs
    vector<pair<char,char>> pairs;
    pairs.reserve(N);
    for (int i = 0; i < m; ) {
        if (i + 1 < m && mate[i] == i + 1) {
            pairs.emplace_back(T[i], T[i+1]);
            i += 2;
        } else {
            // i is unmatched; must be 'A' since all 'C' are matched
            if (i + 1 < m && mate[i+1] == -1 && T[i] == 'A' && T[i+1] == 'A') {
                pairs.emplace_back('A', 'A');
                i += 2;
            } else {
                // Place single A with a dot
                pairs.emplace_back('A', '.');
                i += 1;
            }
        }
    }
    
    // Pad with empty pairs to reach exactly N pairs
    while ((int)pairs.size() < N) pairs.emplace_back('.', '.');
    
    // Concatenate to final string
    string res;
    res.reserve(2 * N);
    for (auto &p : pairs) {
        res.push_back(p.first);
        res.push_back(p.second);
    }
    
    cout << "YES\n" << res << "\n";
    return 0;
}