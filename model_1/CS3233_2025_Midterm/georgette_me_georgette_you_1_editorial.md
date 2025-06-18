### Approach
The problem requires counting the number of permutations of size \( N \) that are not equal to their own inverse permutation. The solution involves two key steps:
1. **Total Permutations**: The total number of permutations of size \( N \) is \( N! \).
2. **Involutions**: These are permutations that are equal to their own inverse. The number of involutions for size \( N \) can be computed using the recurrence relation:
   - \( I(0) = 1 \)
   - \( I(1) = 1 \)
   - \( I(N) = I(N-1) + (N-1) \times I(N-2) \) for \( N \geq 2 \)

The answer for each query \( N \) is then given by \( (N! - I(N)) \mod M \).

### Solution Code
```cpp
#include <iostream>
#include <vector>
using namespace std;

const int MAX_N = 1000000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int T;
    long long M;
    cin >> T >> M;

    vector<long long> fact(MAX_N + 1);
    vector<long long> involution(MAX_N + 1);

    fact[0] = 1;
    for (int i = 1; i <= MAX_N; i++) {
        fact[i] = fact[i - 1] * i % M;
    }

    involution[0] = 1;
    if (MAX_N >= 1) {
        involution[1] = 1;
    }
    for (int i = 2; i <= MAX_N; i++) {
        long long temp = (i - 1) * involution[i - 2] % M;
        involution[i] = (involution[i - 1] + temp) % M;
    }

    while (T--) {
        int N;
        cin >> N;
        long long ans = (fact[N] - involution[N]) % M;
        if (ans < 0) {
            ans += M;
        }
        cout << ans << '\n';
    }
    return 0;
}
```

### Explanation
1. **Precomputation**:
   - **Factorial Array (`fact`)**: Computes \( N! \mod M \) for all \( N \) from 0 to \( 10^6 \) using dynamic programming. The recurrence is \( \text{fact}[0] = 1 \) and \( \text{fact}[i] = \text{fact}[i-1] \times i \mod M \).
   - **Involution Array (`involution`)**: Computes the number of involutions \( I(N) \mod M \) for all \( N \) from 0 to \( 10^6 \) using the recurrence \( I(0) = 1 \), \( I(1) = 1 \), and \( I(N) = I(N-1) + (N-1) \times I(N-2) \mod M \).

2. **Query Processing**:
   - For each query \( N \), the answer is \( (\text{fact}[N] - \text{involution}[N]) \mod M \). If the result is negative, it is adjusted by adding \( M \) to ensure it lies within the range \( [0, M-1] \).

This approach efficiently precomputes necessary values in \( O(\text{MAX\_N}) \) time and processes each query in \( O(1) \) time, making it suitable for large input sizes. The solution correctly handles all possible cases by leveraging modular arithmetic to manage large numbers and ensures correctness through well-known combinatorial recurrences.