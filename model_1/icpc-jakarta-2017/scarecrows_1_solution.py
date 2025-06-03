MOD = 1000000007

import sys

def main():
    data = sys.stdin.read().splitlines()
    if not data: 
        return
    R, C = map(int, data[0].split())
    grid = []
    for i in range(1, 1+R):
        grid.append(data[i].strip())
    
    nT = 1 << R
    base = [0] * C
    for j in range(C):
        mask = 0
        for i in range(R):
            if grid[i][j] == '.':
                mask |= (1 << i)
        base[j] = mask
        
    max_a = R
    power2 = [1] * (max_a + 1)
    for i in range(1, max_a + 1):
        power2[i] = (power2[i-1] * 2) % MOD
        
    popcount_arr = [0] * nT
    for x in range(nT):
        popcount_arr[x] = bin(x).count('1')
        
    F_values = [0] * nT
    
    for T in range(nT):
        non_empty_arr = [0] * C
        for j in range(C):
            avail_mask = base[j] & (~T)
            a_j = popcount_arr[avail_mask]
            if a_j == 0:
                non_empty_arr[j] = 0
            else:
                non_empty_arr[j] = (power2[a_j] - 1) % MOD
                
        if C == 0:
            F_values[T] = 0
        else:
            dp0 = 1
            dp1 = non_empty_arr[0]
            for j in range(1, C):
                new_dp0 = dp1
                new_dp1 = (dp0 + dp1) % MOD
                new_dp1 = (new_dp1 * non_empty_arr[j]) % MOD
                dp0, dp1 = new_dp0, new_dp1
            F_values[T] = (dp0 + dp1) % MOD

    total = 0
    for T in range(nT):
        bits = popcount_arr[T]
        if bits % 2 == 0:
            total = (total + F_values[T]) % MOD
        else:
            total = (total - F_values[T]) % MOD
            
    total %= MOD
    if total < 0:
        total += MOD
    print(total)

if __name__ == '__main__':
    main()