import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    k_val = int(next(it))
    L_val = int(next(it))
    quests = []
    for _ in range(n):
        M = int(next(it))
        X = int(next(it))
        T = int(next(it))
        quests.append((M, X, T))
    
    quests.sort(key=lambda x: (x[0], x[1], x[2]))
    
    MAX_TRAUMA_SUM = 22600
    dp = [[-10**18] * (MAX_TRAUMA_SUM + 1) for _ in range(n + 1)]
    dp[0][0] = 0
    
    for quest in quests:
        m, x, t = quest
        new_dp = [row[:] for row in dp]
        for j in range(n - 1, -1, -1):
            for s_val in range(MAX_TRAUMA_SUM + 1):
                if dp[j][s_val] < m:
                    continue
                new_level = dp[j][s_val] + x
                if new_level > 10**18:
                    new_level = 10**18
                if j < k_val:
                    ns = s_val + t
                    if ns <= MAX_TRAUMA_SUM:
                        if new_level > new_dp[j + 1][ns]:
                            new_dp[j + 1][ns] = new_level
                else:
                    ns = s_val + t
                    if ns <= MAX_TRAUMA_SUM:
                        if new_level > new_dp[j + 1][ns]:
                            new_dp[j + 1][ns] = new_level
        dp = new_dp

    ans = 10**18
    for j in range(1, n + 1):
        for s_val in range(MAX_TRAUMA_SUM + 1):
            if dp[j][s_val] >= L_val:
                if j <= k_val:
                    total_trauma = s_val
                    if total_trauma < ans:
                        ans = total_trauma
                else:
                    total_trauma = s_val
                    if total_trauma < ans:
                        ans = total_trauma
    print(ans)

if __name__ == '__main__':
    main()