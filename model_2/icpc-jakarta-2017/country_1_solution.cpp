#include <bits/stdc++.h>
using namespace std;

static inline vector<array<int,26>> buildNext(const string& s) {
    int m = (int)s.size();
    vector<array<int,26>> nxt(m+1);
    nxt[m].fill(-1);
    for (int i = m-1; i >= 0; --i) {
        nxt[i] = nxt[i+1];
        nxt[i][s[i]-'A'] = i;
    }
    return nxt;
}

// Build the lexicographically smallest K-length subsequence starting with s[0].
static inline bool buildSmallest(const string& s, int K, const vector<array<int,26>>& nxt, string& out) {
    int m = (int)s.size();
    if (m < K) return false;
    string code(K, '?');
    // First letter must be s[0], take it at index 0
    code[0] = s[0];
    int lastIdx = 0;
    int start = lastIdx + 1; // next search starts after lastIdx
    for (int j = 1; j < K; ++j) {
        bool ok = false;
        for (int c = 0; c < 26; ++c) {
            int idx = nxt[start][c];
            if (idx != -1 && (m - 1 - idx) >= (K - 1 - j)) {
                code[j] = char('A' + c);
                lastIdx = idx;
                start = lastIdx + 1;
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    out = code;
    return true;
}

// Build the lexicographically smallest K-length subsequence strictly greater than prev, with first letter fixed (equal to s[0]).
static inline bool buildGreaterThan(const string& s, int K, const vector<array<int,26>>& nxt, const string& prev, string& out) {
    int m = (int)s.size();
    vector<int> posPrefix(K, -1); // earliest positions for prev's prefix
    int start = 0;
    for (int j = 0; j < K; ++j) {
        int c = prev[j] - 'A';
        int idx = nxt[start][c];
        posPrefix[j] = idx;
        if (idx == -1) {
            // cannot match longer prefixes
            for (int jj = j+1; jj < K; ++jj) posPrefix[jj] = -1;
            break;
        }
        start = idx + 1;
    }

    int choose_t = -1, choose_ch = -1, choose_idx = -1;
    for (int t = 1; t < K; ++t) {
        if (posPrefix[t-1] == -1) break; // cannot match prefix up to t-1
        int st = posPrefix[t-1] + 1;
        int fromc = (prev[t] - 'A') + 1;
        for (int c = fromc; c < 26; ++c) {
            int idx = nxt[st][c];
            if (idx != -1 && (m - 1 - idx) >= (K - 1 - t)) {
                choose_t = t; choose_ch = c; choose_idx = idx;
                break;
            }
        }
        if (choose_t != -1) break;
    }
    if (choose_t == -1) return false;

    string code = prev; // prefix [0..choose_t-1] remains the same
    code[choose_t] = char('A' + choose_ch);
    int lastIdx = choose_idx;
    int st = lastIdx + 1;
    for (int j = choose_t + 1; j < K; ++j) {
        bool ok = false;
        for (int c = 0; c < 26; ++c) {
            int idx = nxt[st][c];
            if (idx != -1 && (m - 1 - idx) >= (K - 1 - j)) {
                code[j] = char('A' + c);
                lastIdx = idx;
                st = lastIdx + 1;
                ok = true;
                break;
            }
        }
        if (!ok) return false; // should not happen due to the check above
    }
    out = code;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    if (!(cin >> N >> K)) return 0;
    vector<string> names(N);
    for (int i = 0; i < N; ++i) cin >> names[i];

    vector<pair<string,int>> arr;
    arr.reserve(N);
    for (int i = 0; i < N; ++i) arr.emplace_back(names[i], i);
    sort(arr.begin(), arr.end(),
         [](const pair<string,int>& a, const pair<string,int>& b) {
             return a.first < b.first;
         });

    vector<string> answer(N);
    string prev = "";
    for (int i = 0; i < N; ++i) {
        const string& s = arr[i].first;
        int idx = arr[i].second;
        int m = (int)s.size();
        if (m < K) {
            cout << "NO\n";
            return 0;
        }
        auto nxt = buildNext(s);
        string code;
        if (prev.empty()) {
            if (!buildSmallest(s, K, nxt, code)) {
                cout << "NO\n";
                return 0;
            }
        } else {
            char f = s[0], g = prev[0];
            if (f < g) {
                cout << "NO\n";
                return 0;
            } else if (f > g) {
                if (!buildSmallest(s, K, nxt, code)) {
                    cout << "NO\n";
                    return 0;
                }
            } else {
                if (!buildGreaterThan(s, K, nxt, prev, code)) {
                    cout << "NO\n";
                    return 0;
                }
            }
        }
        answer[idx] = code;
        prev = code;
    }

    cout << "YES\n";
    for (int i = 0; i < N; ++i) {
        cout << answer[i] << "\n";
    }
    return 0;
}