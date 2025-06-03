import sys
from collections import defaultdict

def main():
    data = sys.stdin.read().split()
    t = int(data[0])
    index = 1
    test_cases = []
    for _ in range(t):
        n = int(data[index])
        k = int(data[index+1])
        index += 2
        A = list(map(int, data[index:index+k])) if k > 0 else []
        index += k
        test_cases.append((n, k, A))
    
    groups = defaultdict(list)
    for i, (n, k, A) in enumerate(test_cases):
        groups[n].append((k, A, i))
    
    results = [0.0] * t
    for n, cases in groups.items():
        dp = [[0.0] * (n+1) for _ in range(n+1)]
        for s in range(1, n+1):
            for a in range(0, s+1):
                b = s - a
                if a == 0:
                    dp[0][b] = (n + b * dp[0][b-1]) / b
                elif b == 0:
                    dp[a][0] = (n + a * dp[a-1][1]) / a
                else:
                    dp[a][b] = (n + a * dp[a-1][b+1] + b * dp[a][b-1]) / s
        
        for k_val, A_list, orig_idx in cases:
            freq = [0] * (n+1)
            for x in A_list:
                if 1 <= x <= n:
                    freq[x] += 1
            a_count = 0
            b_count = 0
            for i in range(1, n+1):
                if freq[i] == 0:
                    a_count += 1
                elif freq[i] == 1:
                    b_count += 1
            results[orig_idx] = dp[a_count][b_count]
    
    for res in results:
        print("{:.9f}".format(res))

if __name__ == '__main__':
    main()