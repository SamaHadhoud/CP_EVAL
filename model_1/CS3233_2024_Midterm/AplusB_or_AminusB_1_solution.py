import sys
from functools import lru_cache

def main():
    global L_val, R_val
    L_val, R_val = map(int, sys.stdin.readline().split())
    
    @lru_cache(maxsize=None)
    def dfs(i, carry, tl, th, tb, prev):
        if i == 32:
            return 1 if carry == 0 else 0
        
        L_i = (L_val >> i) & 1
        R_i = (R_val >> i) & 1
        
        res = 0
        low_B = L_i if tl else 0
        high_B = R_i if th else 1
        
        for b in range(low_B, high_B + 1):
            for x in [0, 1]:
                if i > 0 and x == 1 and prev == 1:
                    continue
                
                total = b + x + carry
                bit = total & 1
                new_carry = total // 2
                
                if tb:
                    if bit > R_i:
                        continue
                    new_tb = 1 if (bit == R_i) else 0
                else:
                    new_tb = 0
                
                new_tl = 1 if (tl and (b == L_i)) else 0
                new_th = 1 if (th and (b == R_i)) else 0
                
                res += dfs(i + 1, new_carry, new_tl, new_th, new_tb, b)
        return res
    
    result = dfs(0, 0, 1, 1, 1, 0)
    print(result)

if __name__ == '__main__':
    main()