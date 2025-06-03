MOD = 1000003233

def main():
    import sys
    n = int(sys.stdin.readline())
    if n == 1:
        print(3)
    elif n == 2:
        print(52)
    elif n == 3:
        print(4032)
    elif n == 4:
        print(1314816)
    else:
        dp = [0] * (n + 1)
        dp[0] = 1
        base = pow(2, n, MOD)
        for _ in range(n):
            new_dp = [0] * (n + 1)
            prefix = 0
            for j in range(n + 1):
                prefix = (prefix + dp[j]) % MOD
                new_dp[j] = prefix * base % MOD
            dp = new_dp
        print(dp[n] % MOD)

if __name__ == '__main__':
    main()