import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    T = int(data[0])
    n = int(data[1])
    jobs_by_x = [[] for _ in range(T)]
    index = 2
    for i in range(n):
        x = int(data[index])
        l = int(data[index+1])
        p_val = int(data[index+2])
        index += 3
        r_i = (x + l) % T
        jobs_by_x[x].append((l, p_val, r_i))
    
    M = 5 * T
    INF = 10**18
    dp = [[INF] * T for _ in range(M+1)]
    for r in range(T):
        dp[0][r] = 0

    for k_val in range(1, M+1):
        A = [INF] * T
        for x in range(T):
            for (l, p, r_i) in jobs_by_x[x]:
                if k_val >= p and dp[k_val - p][r_i] != INF:
                    candidate = l + dp[k_val - p][r_i]
                    if candidate < A[x]:
                        A[x] = candidate
        
        B = [INF] * T
        for x in range(T):
            if A[x] < INF:
                B[x] = A[x] + x
            else:
                B[x] = INF

        suff_min = [INF] * T
        if T > 0:
            suff_min[T-1] = B[T-1]
            for x in range(T-2, -1, -1):
                suff_min[x] = min(B[x], suff_min[x+1])
        
        P = [INF] * T
        if T > 0:
            P[0] = INF
            for x in range(1, T):
                prev_val = B[x-1] + T if B[x-1] != INF else INF
                P[x] = min(P[x-1], prev_val)
        
        for r in range(T):
            candidate = min(suff_min[r], P[r])
            if candidate < INF:
                dp[k_val][r] = candidate - r
            else:
                dp[k_val][r] = INF

    best_cycle_profit = [0] * T
    best_cycle_time = [INF] * T
    points = [[] for _ in range(T)]
    for r in range(T):
        for k_val in range(0, M+1):
            if dp[k_val][r] != INF:
                points[r].append((dp[k_val][r], k_val))
    
    for r in range(T):
        if not points[r]:
            best_cycle_profit[r] = 0
            best_cycle_time[r] = INF
            continue
        
        points[r].sort(key=lambda x: (x[0], -x[1]))
        unique_points = []
        last_t = -1
        for i in range(len(points[r])):
            t, k_val = points[r][i]
            if i == 0 or t != last_t:
                unique_points.append((t, k_val))
                last_t = t
        
        filtered = []
        current_max_k = -1
        for t, k_val in unique_points:
            if k_val > current_max_k:
                filtered.append((t, k_val))
                current_max_k = k_val
        
        if len(filtered) < 2:
            best_cycle_profit[r] = 0
            best_cycle_time[r] = INF
        else:
            best_cycle_pro = 0
            best_cycle_tim = INF
            for i in range(len(filtered) - 1):
                t1, k1 = filtered[i]
                t2, k2 = filtered[i+1]
                profit = k2 - k1
                time_diff = t2 - t1
                if time_diff <= 0:
                    continue
                if best_cycle_tim == INF:
                    best_cycle_pro = profit
                    best_cycle_tim = time_diff
                else:
                    if profit * best_cycle_tim > best_cycle_pro * time_diff:
                        best_cycle_pro = profit
                        best_cycle_tim = time_diff
                    elif profit * best_cycle_tim == best_cycle_pro * time_diff:
                        if time_diff < best_cycle_tim:
                            best_cycle_pro = profit
                            best_cycle_tim = time_diff
            best_cycle_profit[r] = best_cycle_pro
            best_cycle_time[r] = best_cycle_tim

    q = int(data[index])
    index += 1
    queries = []
    for i in range(q):
        queries.append(int(data[index]))
        index += 1

    states = []
    for r in range(T):
        for k_val in range(M+1):
            if dp[k_val][r] != INF:
                states.append((r, k_val, dp[k_val][r]))
    
    out_lines = []
    for z in queries:
        ans = 0
        for (r, k_val, time_val) in states:
            if time_val > z:
                continue
            if best_cycle_time[r] == INF:
                total = k_val
            else:
                q_cycles = (z - time_val) // best_cycle_time[r]
                total = k_val + best_cycle_profit[r] * q_cycles
            if total > ans:
                ans = total
        out_lines.append(str(ans))
    
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()