### Problem Analysis
The problem involves rearranging students seated in a long table with $2N$ seats arranged in $N$ adjacent pairs. The goal is to ensure that every student from the School of Computing ('C') has a buddy (adjacent seat in the same pair) from the Faculty of Arts and Social Sciences ('A'). The Dean can move a student to an adjacent empty seat ('.') any number of times. The solution must determine if such a rearrangement is possible and provide one valid arrangement if it is.

#### Key Insights
1. **Necessary Condition**: The number of 'A' students must be at least the number of 'C' students. This is because each 'C' must be paired with an 'A' in the same pair, and no pair can have more than one 'C' (since a pair with two 'C's would leave both without an 'A' buddy).
2. **Global Constraints**: Even if the total number of 'A's is sufficient, the arrangement must ensure that at no point in the sequence (from left to right or right to left) does the number of 'C's encountered exceed the number of available 'A's plus the number of empty seats that can be used as buffers (to bring 'A's from other parts of the table).
3. **Rearrangement Feasibility**: Using empty seats as buffers, we can move students to achieve any arrangement as long as the necessary conditions are met. The feasibility is checked via two scans (forward and backward) that ensure local constraints are satisfied by using empty seats when needed.

### Approach
1. **Initial Checks**:
   - Count the total number of 'A's, 'C's, and '.'s in the initial arrangement.
   - If the number of 'A's is less than the number of 'C's, output "NO" immediately since it's impossible to satisfy the buddy condition.

2. **Forward and Backward Scans**:
   - **Forward Scan (left to right)**: Track a balance that increases for each 'A' and decreases for each 'C'. If the balance becomes negative, use an available "credit" (representing an empty seat that can be used to bring an 'A') to reset the balance. If no credit is available, the arrangement is impossible.
   - **Backward Scan (right to left)**: Repeat the same process in reverse, using a reset credit count. This ensures that the arrangement is feasible from both directions.

3. **Constructing a Valid Arrangement**:
   - If both scans pass, output "YES" and construct the arrangement:
     - Place each 'C' in a distinct pair with an 'A' (using the first `total_C` pairs as "AC").
     - Fill the remaining pairs with the leftover 'A's and '.'s, ensuring no 'C' appears in these pairs.

### Solution Code
```python
def main():
    import sys
    data = sys.stdin.read().splitlines()
    n = int(data[0].strip())
    s = data[1].strip()
    
    total_A = s.count('A')
    total_C = s.count('C')
    empty_seats = s.count('.')
    total_seats = 2 * n
    
    if total_A < total_C:
        print("NO")
        return
    
    credit = total_A - total_C
    balance = 0
    for char in s:
        if char == 'A':
            balance += 1
        elif char == 'C':
            balance -= 1
            if balance < 0:
                if credit > 0:
                    credit -= 1
                    balance += 1
                else:
                    print("NO")
                    return
    
    credit = total_A - total_C
    balance = 0
    for char in reversed(s):
        if char == 'A':
            balance += 1
        elif char == 'C':
            balance -= 1
            if balance < 0:
                if credit > 0:
                    credit -= 1
                    balance += 1
                else:
                    print("NO")
                    return
    
    print("YES")
    res = []
    for _ in range(total_C):
        res.append('A')
        res.append('C')
    
    remaining_pairs = n - total_C
    remaining_A = total_A - total_C
    remaining_seats = 2 * remaining_pairs
    for i in range(remaining_seats):
        if i < remaining_A:
            res.append('A')
        else:
            res.append('.')
    
    print(''.join(res))

if __name__ == '__main__':
    main()
```

### Explanation
1. **Initial Checks**: The code first checks if the number of 'A's is at least the number of 'C's. If not, it outputs "NO".
2. **Forward Scan**: The code scans the string from left to right, maintaining a balance. For each 'A', the balance increases; for each 'C', it decreases. If the balance becomes negative, it uses a credit (from available empty seats) to adjust the balance. If no credit is available, it outputs "NO".
3. **Backward Scan**: The same process is repeated from right to left to ensure feasibility in both directions.
4. **Output Arrangement**: If both scans pass, the code constructs a valid arrangement:
   - The first `total_C` pairs are filled with "AC" to ensure each 'C' has an 'A' buddy.
   - The remaining pairs are filled with leftover 'A's followed by '.'s, ensuring no 'C's are in these pairs.
   
This approach efficiently checks feasibility and constructs a valid arrangement while respecting the constraints, ensuring optimal performance with $O(N)$ time complexity.