MOD = 998244353
MAX_N = 300000

def main():
    import sys
    data = sys.stdin.read().split()
    t = int(data[0])
    ns = list(map(int, data[1:1+t]))
    max_n = max(ns) if ns else 0
    
    f_arr = [0] * (max_n + 1)
    if max_n >= 1:
        f_arr[1] = 1
    if max_n >= 2:
        f_arr[2] = 1
    for n in range(3, max_n + 1):
        add = 1 if n % 3 == 0 else 0
        f_arr[n] = (f_arr[n-1] + f_arr[n-2] + add) % MOD
    
    out_lines = []
    for n in ns:
        out_lines.append(str(f_arr[n]))
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()