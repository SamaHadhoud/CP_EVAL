MOD = 998244353

def main():
    import sys
    r, c = map(int, sys.stdin.readline().split())
    a = r // 6
    b = c // 7
    exponent = 42 * a * b
    result = pow(2, exponent, MOD)
    print(result)

if __name__ == '__main__':
    main()