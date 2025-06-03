import sys

INF = 10**18
NEG_INF = -10**18
S = 10

def main():
    data = sys.stdin.read().split()
    t = int(data[0])
    index = 1
    out_lines = []
    for _ in range(t):
        n = int(data[index])
        index += 1
        p = list(map(int, data[index:index + n]))
        index += n
        a_list = list(map(int, data[index:index + n]))
        index += n

        max1_list = [None]
        min0_list = [None]
        for d in range(1, S + 1):
            max1_list.append([NEG_INF] * d)
            min0_list.append([INF] * d)
        
        dp0 = [0] * (n + 1)
        dp1 = [0] * (n + 1)
        
        for i in range(n, 0, -1):
            a_i = a_list[i - 1]
            d_val = p[i - 1]
            candidate0 = None
            candidate1 = None
            
            if d_val <= S:
                r = i % d_val
                if max1_list[d_val][r] > NEG_INF:
                    candidate0 = max1_list[d_val][r]
                    candidate1 = min0_list[d_val][r]
            else:
                j = i + d_val
                c0 = NEG_INF
                c1 = INF
                found = False
                while j <= n:
                    found = True
                    if dp1[j] > c0:
                        c0 = dp1[j]
                    if dp0[j] < c1:
                        c1 = dp0[j]
                    j += d_val
                if found:
                    candidate0 = c0
                    candidate1 = c1
            
            if candidate0 is not None:
                dp0[i] = max(a_i, a_i + candidate0)
                dp1[i] = min(a_i, a_i + candidate1)
            else:
                dp0[i] = a_i
                dp1[i] = a_i
            
            for d in range(1, S + 1):
                r = i % d
                if dp1[i] > max1_list[d][r]:
                    max1_list[d][r] = dp1[i]
                if dp0[i] < min0_list[d][r]:
                    min0_list[d][r] = dp0[i]
        
        res_arr = [str(dp0[i]) for i in range(1, n + 1)]
        out_lines.append(" ".join(res_arr))
    
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()