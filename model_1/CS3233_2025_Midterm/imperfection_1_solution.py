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
        if index > self.n:
            index = self.n
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
        suffix_sum[i] = suffix_sum[i+1] + sum_cost[i]
    
    total_count_below = [0] * (MAX_V + 2)
    for x in range(1, MAX_V + 1):
        total_count_below[x] = total_count_below[x-1] + F[x-1]
    
    prefix_x_arr = [None] * (MAX_V + 2)
    for v in range(1, MAX_V + 1):
        if F[v] > 0:
            lst = sorted(costs_by_value[v])
            arr = [0] * (F[v] + 1)
            for i in range(1, F[v] + 1):
                arr[i] = arr[i-1] + lst[i-1]
            prefix_x_arr[v] = arr
    
    cnt_tree = Fenw(MAX_C)
    sum_tree = Fenw(MAX_C)
    candidate_ans = 10**18
    
    for x in range(1, MAX_V + 1):
        if x >= 2:
            for c in costs_by_value[x-1]:
                cnt_tree.update(c, 1)
                sum_tree.update(c, c)
                
        if F[x] == 0:
            continue
            
        fixed_cost = suffix_sum[x+1]
        k0 = F[x]
        t0 = total_count_below[x]
        
        if k0 > t0:
            total_cost = fixed_cost
            if total_cost < candidate_ans:
                candidate_ans = total_cost
        else:
            best_var = 10**18
            for b in range(0, k0):
                a_min = t0 - k0 + b + 1
                if a_min > t0:
                    break
                low, high = 1, MAX_C
                while low < high:
                    mid = (low + high) // 2
                    if cnt_tree.query(mid) >= a_min:
                        high = mid
                    else:
                        low = mid + 1
                T = low
                cnt1 = cnt_tree.query(T-1)
                sum1 = sum_tree.query(T-1)
                nonx_cost = sum1 + T * (a_min - cnt1)
                x_cost = prefix_x_arr[x][b]
                total_var = nonx_cost + x_cost
                if total_var < best_var:
                    best_var = total_var
                    
            total_cost = fixed_cost + best_var
            if total_cost < candidate_ans:
                candidate_ans = total_cost
                
    print(candidate_ans)

if __name__ == '__main__':
    main()