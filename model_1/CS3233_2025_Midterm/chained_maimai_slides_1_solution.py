import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    M = int(data[0])
    K = int(data[1])
    edges = []
    index = 2
    for i in range(M):
        a = int(data[index]); b = int(data[index+1]); s_val = int(data[index+2])
        index += 3
        edges.append((a, b, s_val))
    
    edges_sorted = sorted(edges, key=lambda x: x[2])
    
    BOUND = 20
    INF = 10**18
    ans = INF
    
    for L in range(M):
        dp = {}
        init_state = (0, 0, 0, 0, 0, 0, 0, 0)
        dp[init_state] = 0
        found_ans_here = False
        for R in range(L, M):
            a, b, speed_val = edges_sorted[R]
            new_dp = dp.copy()
            for state, count_edges in list(dp.items()):
                f0, f1, f2, f3, f4, f5, f6, mask_val = state
                f_list = [f0, f1, f2, f3, f4, f5, f6]
                new_f = f_list.copy()
                if 1 <= a <= 7:
                    new_f[a-1] += 1
                if 1 <= b <= 7:
                    new_f[b-1] -= 1
                if any(abs(x) > BOUND for x in new_f):
                    continue
                new_mask = mask_val | (1 << (a-1))
                new_state_tuple = (new_f[0], new_f[1], new_f[2], new_f[3], new_f[4], new_f[5], new_f[6], new_mask)
                new_count = count_edges + 1
                if new_state_tuple in new_dp:
                    if new_count > new_dp[new_state_tuple]:
                        new_dp[new_state_tuple] = new_count
                else:
                    new_dp[new_state_tuple] = new_count
            dp = new_dp
            final_state = (0, 0, 0, 0, 0, 0, 0, 255)
            if final_state in dp and dp[final_state] >= K:
                current_range = edges_sorted[R][2] - edges_sorted[L][2]
                if current_range < ans:
                    ans = current_range
                found_ans_here = True
                break
        if found_ans_here:
            continue
    if ans == INF:
        print(-1)
    else:
        print(ans)

if __name__ == "__main__":
    main()