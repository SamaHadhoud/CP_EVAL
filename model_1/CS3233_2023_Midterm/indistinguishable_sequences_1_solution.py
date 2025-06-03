MOD = 1000003233

def main():
    import sys
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    a = list(map(int, data[1:1+n]))
    
    if n == 0:
        print(0)
        return

    size = n
    fenw_f = [0] * (size + 1)
    
    def fenw_update_max(fenw, size, i, val):
        while i <= size:
            if val > fenw[i]:
                fenw[i] = val
            i += i & -i

    def fenw_query_max(fenw, i):
        res = 0
        while i:
            if fenw[i] > res:
                res = fenw[i]
            i -= i & -i
        return res

    f_arr = [1] * n
    for i in range(n):
        val = a[i]
        if val > 1:
            q = fenw_query_max(fenw_f, val - 1)
        else:
            q = 0
        f_arr[i] = q + 1
        fenw_update_max(fenw_f, size, val, f_arr[i])
    
    c = [size + 1 - x for x in a]
    fenw_g = [0] * (size + 1)
    g_arr = [1] * n
    for i in range(n - 1, -1, -1):
        val = c[i]
        if val > 1:
            q = fenw_query_max(fenw_g, val - 1)
        else:
            q = 0
        g_arr[i] = q + 1
        fenw_update_max(fenw_g, size, val, g_arr[i])
    
    L_val = max(f_arr) if n > 0 else 0
    
    F_indices = []
    for i in range(n):
        if f_arr[i] + g_arr[i] - 1 == L_val:
            F_indices.append(i)
    total_critical = len(F_indices)
    non_critical = n - total_critical

    levels = [[] for _ in range(L_val + 1)]
    for i in F_indices:
        lev = f_arr[i]
        if lev <= L_val:
            levels[lev].append(a[i])
    
    for k in range(1, L_val + 1):
        levels[k].sort(reverse=True)
    
    pow2 = [1] * (n + 1)
    for i in range(1, n + 1):
        pow2[i] = (pow2[i - 1] * 2) % MOD
        
    inv2 = pow(2, MOD - 2, MOD)
    inv_pow2 = [1] * (n + 1)
    for i in range(1, n + 1):
        inv_pow2[i] = (inv_pow2[i - 1] * inv2) % MOD

    dp0 = 1
    dp = [0] * (n + 1)
    
    for k in range(1, L_val + 1):
        level_vals = levels[k]
        total_in_level = len(level_vals)
        if total_in_level == 0:
            continue
            
        cnt_prefix_arr = [0] * (n + 1)
        for val in level_vals:
            if 1 <= val <= n:
                cnt_prefix_arr[val] += 1
        for i in range(1, n + 1):
            cnt_prefix_arr[i] += cnt_prefix_arr[i - 1]
        
        F_positive = [0] * (n + 1)
        for s in range(1, n + 1):
            F_positive[s] = dp[s] * pow2[cnt_prefix_arr[s]] % MOD
        
        suffix_sum = [0] * (n + 2)
        for s in range(n, 0, -1):
            suffix_sum[s] = (suffix_sum[s + 1] + F_positive[s]) % MOD

        next_dp_level = [0] * (n + 1)
        for v in level_vals:
            if v < 1 or v > n:
                continue
            term0 = dp0 * pow2[total_in_level - cnt_prefix_arr[v]] % MOD
            term_positive = suffix_sum[v] * inv_pow2[cnt_prefix_arr[v]] % MOD
            next_dp_level[v] = (term0 + term_positive) % MOD
        
        new_dp0 = dp0
        new_dp = [0] * (n + 1)
        for s in range(1, n + 1):
            new_dp[s] = dp[s]
        for v in level_vals:
            if 1 <= v <= n:
                new_dp[v] = (new_dp[v] + next_dp_level[v]) % MOD
        
        dp0 = new_dp0
        dp = new_dp

    total_valid = dp0
    for i in range(1, n + 1):
        total_valid = (total_valid + dp[i]) % MOD

    total_subsets = pow2[n]
    non_critical_power = pow2[non_critical]
    result = (total_subsets - non_critical_power * total_valid) % MOD
    if result < 0:
        result += MOD
    print(result)

if __name__ == '__main__':
    main()