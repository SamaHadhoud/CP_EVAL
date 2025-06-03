import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    T = int(data[0])
    M = int(data[1])
    queries = list(map(int, data[2:2+T]))
    
    max_n = 1000000
    fact_arr = [1] * (max_n + 1)
    for i in range(1, max_n + 1):
        fact_arr[i] = fact_arr[i - 1] * i % M
        
    involution_arr = [1] * (max_n + 1)
    if max_n >= 1:
        involution_arr[1] = 1
    for n in range(2, max_n + 1):
        term = (n - 1) * involution_arr[n - 2] % M
        involution_arr[n] = (involution_arr[n - 1] + term) % M
        
    out_lines = []
    for N in queries:
        ans = (fact_arr[N] - involution_arr[N]) % M
        if ans < 0:
            ans += M
        out_lines.append(str(ans))
    
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()