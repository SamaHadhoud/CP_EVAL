### Approach
The problem requires counting the number of positive integers \( n \) (where \( 1 \leq n \leq X \)) such that both \( n \) and \( n^2 \) have non-decreasing digits in base 10. Given the constraints \( X \leq 10^{18} \) and \( T \leq 10^5 \), a brute-force approach is infeasible. Instead, we leverage the observation that the set of numbers with non-decreasing digits is relatively small (about 4.6 million for \( n \leq 10^{18} \)).

1. **Generate Sorted Numbers**: We generate all numbers \( n \) with non-decreasing digits up to \( 10^{18} \). This is done using a BFS approach, processing numbers by their digit length (from 1 to 18 digits). For each number, the next digit must be at least the last digit to maintain the non-decreasing property.
2. **Check Valid Numbers**: For each generated number \( n \), compute \( n^2 \) and check if its digits are non-decreasing. This involves:
   - **Computing \( n^2 \)**: Using 128-bit integers to handle large squares (up to \( 10^{36} \)).
   - **Digit Check**: Convert \( n^2 \) to a string and verify that each digit is less than or equal to the next digit.
3. **Answer Queries**: Store all valid \( n \) in a sorted list. For each query \( X \), use binary search to count how many valid numbers are \( \leq X \).

### Solution Code
```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <tuple>
using namespace std;

typedef unsigned long long ull;
#ifdef __SIZEOF_INT128__
typedef __int128 lll;
#else
#error "__int128 is not supported"
#endif

string to_string(lll num) {
    if (num == 0) return "0";
    string s;
    while (num) {
        s = char('0' + (num % 10)) + s;
        num /= 10;
    }
    return s;
}

bool is_sorted(const string& s) {
    for (int i = 0; i < (int)s.size() - 1; i++) {
        if (s[i] > s[i+1])
            return false;
    }
    return true;
}

int main() {
    vector<ull> valid_n;
    const ull maxX = 1000000000000000000ULL; // 10^18

    queue<tuple<ull, int, int>> q; // (current number, last digit, length)
    for (int d = 1; d <= 9; d++) {
        q.push(make_tuple(d, d, 1));
    }

    while (!q.empty()) {
        auto [cur, last, len] = q.front(); q.pop();
        // Check if n and n^2 are sorted
        lll nsq = (lll)cur * cur;
        string sq_str = to_string(nsq);
        if (is_sorted(sq_str)) {
            valid_n.push_back(cur);
        }

        if (len < 18) {
            for (int d = last; d <= 9; d++) {
                ull next_num = cur * 10ULL + (ull)d;
                if (next_num > maxX) break;
                q.push(make_tuple(next_num, d, len+1));
            }
        }
    }

    sort(valid_n.begin(), valid_n.end());

    int T;
    cin >> T;
    while (T--) {
        ull X;
        cin >> X;
        auto it = upper_bound(valid_n.begin(), valid_n.end(), X);
        cout << (it - valid_n.begin()) << '\n';
    }
    return 0;
}
```

### Explanation
1. **Generating Sorted Numbers**: The BFS starts with single-digit numbers (1-9). For each number, new numbers are formed by appending digits that are at least the last digit (to maintain non-decreasing digits). This ensures all generated numbers have non-decreasing digits.
2. **Checking Valid Numbers**: For each generated number \( n \), \( n^2 \) is computed using 128-bit integers to avoid overflow. The square is converted to a string, and its digits are checked for non-decreasing order.
3. **Handling Queries**: Valid numbers are stored in a sorted list. For each query \( X \), the count of valid numbers \( \leq X \) is found using binary search, ensuring efficient responses even for large \( T \).

This approach efficiently handles the constraints by leveraging the limited set of sorted numbers and binary search for quick query resolution. The precomputation ensures that each query is answered in logarithmic time relative to the number of valid numbers (which is small, around 110 for \( X \leq 10^{18} \)).