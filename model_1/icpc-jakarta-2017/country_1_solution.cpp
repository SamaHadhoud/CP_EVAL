#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>

using namespace std;

map<char, vector<int>> build_pos_map(const string& s) {
    map<char, vector<int>> mp;
    for (int i = 0; i < s.size(); i++) {
        mp[s[i]].push_back(i);
    }
    return mp;
}

string greedy_suffix(const string& S, int start, int length, const map<char, vector<int>>& pos_map, int n) {
    if (length == 0) {
        return "";
    }
    string res;
    int current = start;
    for (int j = 0; j < length; j++) {
        int R_bound = n - (length - j - 1);
        if (current > R_bound) {
            return "";
        }
        char found_char = 0;
        int found_index = n + 10000;
        for (char c = 'A'; c <= 'Z'; c++) {
            auto it = pos_map.find(c);
            if (it == pos_map.end()) {
                continue;
            }
            const vector<int>& arr = it->second;
            auto it2 = lower_bound(arr.begin(), arr.end(), current);
            if (it2 != arr.end() && *it2 <= R_bound) {
                found_char = c;
                found_index = *it2;
                break;
            }
        }
        if (found_char == 0) {
            return "";
        }
        res += found_char;
        current = found_index + 1;
    }
    return res;
}

string find_code_geq(const string& S, const string& prev, int K, const map<char, vector<int>>& pos_map) {
    int n = S.size();
    if (n < K) {
        return "";
    }
    if (S[0] < prev[0]) {
        return "";
    }
    string res = "";
    res += S[0];
    int current_index = 0;
    bool bound = (S[0] == prev[0]);

    for (int j = 1; j < K; j++) {
        int L = current_index + 1;
        int R_bound = n - (K - j);
        if (L > R_bound) {
            return "";
        }

        char low_bound = bound ? prev[j] : 'A';
        char found_char = 0;
        int found_index = n + 10000;
        for (char c = low_bound; c <= 'Z'; c++) {
            auto it = pos_map.find(c);
            if (it == pos_map.end()) {
                continue;
            }
            const vector<int>& arr = it->second;
            auto it2 = lower_bound(arr.begin(), arr.end(), L);
            if (it2 != arr.end() && *it2 <= R_bound) {
                found_char = c;
                found_index = *it2;
                break;
            }
        }
        if (found_char == 0) {
            return "";
        }
        res += found_char;
        current_index = found_index;
        if (bound) {
            if (found_char > prev[j]) {
                bound = false;
            }
        }
    }
    return res;
}

string next_code_helper(const string& S, const string& prev, int K, const map<char, vector<int>>& pos_map) {
    int n = S.size();
    vector<int> occ;
    int current_index = -1;
    for (int i = 0; i < K; i++) {
        char c = prev[i];
        auto it = pos_map.find(c);
        if (it == pos_map.end()) {
            return "";
        }
        const vector<int>& arr = it->second;
        auto it2 = lower_bound(arr.begin(), arr.end(), current_index + 1);
        if (it2 == arr.end()) {
            return "";
        }
        int pos_val = *it2;
        occ.push_back(pos_val);
        current_index = pos_val;
    }

    for (int i = K - 1; i >= 0; i--) {
        int L_bound;
        if (i == 0) {
            L_bound = 0;
        } else {
            L_bound = occ[i - 1] + 1;
        }
        int R_bound = n - (K - i - 1);
        if (L_bound > R_bound) {
            continue;
        }

        char current_char = prev[i];
        for (char c = current_char + 1; c <= 'Z'; c++) {
            auto it = pos_map.find(c);
            if (it == pos_map.end()) {
                continue;
            }
            const vector<int>& arr = it->second;
            auto it2 = lower_bound(arr.begin(), arr.end(), L_bound);
            if (it2 == arr.end() || *it2 > R_bound) {
                continue;
            }
            int pos_val = *it2;
            string suffix = greedy_suffix(S, pos_val + 1, K - i - 1, pos_map, n);
            if (suffix != "") {
                string new_code = prev.substr(0, i) + string(1, c) + suffix;
                return new_code;
            }
        }
    }
    return "";
}

int main() {
    int N, K_val;
    cin >> N >> K_val;
    vector<string> country_names;
    string line;
    getline(cin, line);
    for (int i = 0; i < N; i++) {
        getline(cin, line);
        country_names.push_back(line);
    }

    vector<map<char, vector<int>>> pos_maps;
    for (int i = 0; i < N; i++) {
        pos_maps.push_back(build_pos_map(country_names[i]));
    }

    vector<pair<string, int>> sorted_countries;
    for (int i = 0; i < N; i++) {
        sorted_countries.push_back({country_names[i], i});
    }
    sort(sorted_countries.begin(), sorted_countries.end());

    vector<string> ans(N, "");
    string prev_code = "";

    for (int idx = 0; idx < N; idx++) {
        string name = sorted_countries[idx].first;
        int orig_index = sorted_countries[idx].second;
        const map<char, vector<int>>& pm = pos_maps[orig_index];

        if (idx == 0) {
            string bound0 = string(1, name[0]);
            if (K_val > 1) {
                bound0 += string(K_val - 1, 'A');
            }
            string code = find_code_geq(name, bound0, K_val, pm);
            if (code == "") {
                cout << "NO" << endl;
                return 0;
            }
            ans[orig_index] = code;
            prev_code = code;
        } else {
            string candidate = find_code_geq(name, prev_code, K_val, pm);
            if (candidate == "") {
                cout << "NO" << endl;
                return 0;
            }
            if (candidate > prev_code) {
                ans[orig_index] = candidate;
                prev_code = candidate;
            } else {
                string next_candidate = next_code_helper(name, prev_code, K_val, pm);
                if (next_candidate == "") {
                    cout << "NO" << endl;
                    return 0;
                }
                ans[orig_index] = next_candidate;
                prev_code = next_candidate;
            }
        }
    }

    cout << "YES" << endl;
    for (int i = 0; i < N; i++) {
        cout << ans[i] << endl;
    }

    return 0;
}