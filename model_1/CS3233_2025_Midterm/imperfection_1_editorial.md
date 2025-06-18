### Approach
The problem requires deleting some elements from an array such that the resulting array is "perfect." A perfect array is defined as one where the maximum element is also the majority element, meaning its frequency is strictly greater than the sum of the frequencies of all other elements in the array.

#### Key Insights
1. **Candidate Selection**: The maximum element in the resulting array must be one of the distinct values present in the original array. For each candidate value \( x \), we consider it as the maximum in the resulting array.
2. **Deletion Strategy**:
   - **Step 1**: Delete all elements greater than \( x \) to ensure \( x \) is the maximum.
   - **Step 2**: Adjust the remaining elements (those \(\leq x\)) so that the frequency of \( x \) is strictly greater than the combined frequency of all other elements. This involves deleting some non-\( x \) elements (values less than \( x \)) and possibly some \( x \) elements to meet the condition efficiently.
3. **Cost Calculation**:
   - **Fixed Cost**: The cost of deleting elements greater than \( x \) is precomputed using a suffix sum array.
   - **Variable Cost**: For the remaining elements, compute the minimal cost to delete elements such that the condition \( \text{count}(x) > \text{count}(\text{non-}x) \) holds. This involves:
     - Precomputing sorted lists of deletion costs for each value \( x \) and their prefix sums.
     - Maintaining Fenwick Trees (Binary Indexed Trees) to dynamically track and query the smallest deletion costs for non-\( x \) elements as we iterate through candidate values.

#### Algorithm
1. **Precomputation**:
   - **Frequency and Cost Arrays**: For each value, store its frequency and list of deletion costs.
   - **Suffix Sum Array**: Compute the cumulative cost of deleting all elements greater than any given value \( x \).
   - **Prefix Count Array**: Compute the cumulative count of elements less than any given value \( x \).
   - **Sorted Cost Lists**: For each value \( x \), sort its deletion costs and compute prefix sums for quick access.
2. **Processing Candidates**:
   - Iterate over each candidate value \( x \) from 1 to the maximum value in the array.
   - For each \( x \):
     - **Update Fenwick Trees**: Add elements with value \( x-1 \) to the trees (to include all values less than \( x \)).
     - **Fixed Cost**: Retrieve the cost of deleting elements greater than \( x \).
     - **Condition Check**: If the initial frequency of \( x \) already satisfies the majority condition, the variable cost is zero.
     - **Variable Cost Calculation**: If not, iterate over possible deletions of \( x \) elements (from 0 to \( \text{count}(x)-1 \)), compute the required deletions of non-\( x \) elements, and use the Fenwick Trees to find the minimal cost for these deletions.
3. **Result Extraction**: Track the minimal total cost (fixed + variable) across all candidates.

#### Complexity Analysis
- **Time Complexity**: \( O(N \log M) \), where \( N \) is the number of elements (up to \( 3 \times 10^5 \)) and \( M \) is the maximum cost (up to \( 3 \times 10^5 \)). The solution involves processing each candidate and performing logarithmic-time operations for Fenwick Tree updates and queries.
- **Space Complexity**: \( O(N + M) \), due to storage for frequency arrays, cost lists, prefix sums, and Fenwick Trees.

### Solution Code
```python
import sys

class Fenw:
    def __init__(self, size):
        self.n = size
        self.tree = [0] * (self.n + 1)
    
    def update(self, index, delta):
        while index <= self.n:
            self.tree[index] += delta
            index += index & -index
            
    def query(self, index):
        if index <= 0:
            return 0
        s = 0
        while index:
            s += self.tree[index]
            index -= index & -index
        return s

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    A = list(map(int, data[1:1+n]))
    B = list(map(int, data[1+n:1+2*n]))
    
    MAX_V = 300000
    MAX_C = 300000
    
    F = [0] * (MAX_V + 2)
    sum_cost = [0] * (MAX_V + 2)
    costs_by_value = [[] for _ in range(MAX_V + 2)]
    
    for i in range(n):
        a_val = A[i]
        b_val = B[i]
        if a_val > MAX_V:
            continue
        F[a_val] += 1
        sum_cost[a_val] += b_val
        costs_by_value[a_val].append(b_val)
    
    suffix_sum = [0] * (MAX_V + 3)
    for i in range(MAX_V, 0, -1):
        suffix_sum[i] = suffix_sum[i + 1] + sum_cost[i]
    
    total_count_below = [0] * (MAX_V + 2)
    for x in range(1, MAX_V + 1):
        total_count_below[x] = total_count_below[x - 1] + F[x - 1]
    
    prefix_x_arr = [None] * (MAX_V + 2)
    for v in range(1, MAX_V + 1):
        if F[v] > 0:
            lst = sorted(costs_by_value[v])
            arr = [0] * (F[v] + 1)
            for i in range(1, F[v] + 1):
                arr[i] = arr[i - 1] + lst[i - 1]
            prefix_x_arr[v] = arr
    
    cnt_tree = Fenw(MAX_C)
    sum_tree = Fenw(MAX_C)
    candidate_ans = 10**18
    
    for x in range(1, MAX_V + 1):
        if x >= 2:
            for c in costs_by_value[x - 1]:
                cnt_tree.update(c, 1)
                sum_tree.update(c, c)
                
        if F[x] == 0:
            continue
            
        fixed_cost = suffix_sum[x + 1]
        k0 = F[x]
        t0 = total_count_below[x]
        D0 = max(0, t0 - k0 + 1)
        
        if D0 == 0:
            total_cost = fixed_cost
            if total_cost < candidate_ans:
                candidate_ans = total_cost
        else:
            best_step2 = 10**18
            for b in range(0, k0):
                a = D0 + b
                if a > t0:
                    break
                    
                low, high = 1, MAX_C
                while low < high:
                    mid = (low + high) // 2
                    if cnt_tree.query(mid) >= a:
                        high = mid
                    else:
                        low = mid + 1
                T = low
                cnt1 = cnt_tree.query(T - 1)
                sum1 = sum_tree.query(T - 1)
                s_nonx = sum1 + T * (a - cnt1)
                
                s_x = prefix_x_arr[x][b]
                total_step2 = s_nonx + s_x
                if total_step2 < best_step2:
                    best_step2 = total_step2
                    
            total_cost = fixed_cost + best_step2
            if total_cost < candidate_ans:
                candidate_ans = total_cost
                
    print(candidate_ans)

if __name__ == '__main__':
    main()
```

### Explanation
1. **Initialization and Input Handling**: The code reads the input values for the array size, elements, and deletion costs.
2. **Frequency and Cost Processing**: It calculates the frequency of each value and the total deletion cost per value, storing these in arrays.
3. **Suffix Sum Array**: Computes the cumulative cost of deleting all elements greater than any value \( x \), facilitating quick lookup during candidate processing.
4. **Prefix Count Array**: Tracks the cumulative count of elements less than each candidate \( x \), used to determine the initial conditions for the majority check.
5. **Sorted Cost Lists**: For each value, the deletion costs are sorted, and prefix sums are precomputed to efficiently calculate the cost of deleting a certain number of elements.
6. **Fenwick Trees for Dynamic Tracking**: These trees dynamically maintain the count and sum of deletion costs for elements less than the current candidate \( x \), allowing efficient range queries and updates.
7. **Candidate Processing**: For each candidate \( x \):
   - Elements with value \( x-1 \) are added to the Fenwick Trees.
   - The fixed cost of deleting elements greater than \( x \) is retrieved.
   - If the initial frequency of \( x \) meets the majority condition, the variable cost is zero.
   - Otherwise, the code iterates over possible deletions of \( x \) elements, computes the required deletions of non-\( x \) elements, and uses the Fenwick Trees to find the minimal cost for these deletions.
8. **Result Extraction**: The minimal total cost across all candidates is printed as the solution.

This approach efficiently handles the constraints by leveraging precomputation and dynamic data structures to minimize both time and space complexity.