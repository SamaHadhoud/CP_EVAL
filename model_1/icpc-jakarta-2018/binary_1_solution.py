import sys

def main():
    data = sys.stdin.read().splitlines()
    if not data: 
        return
    K = int(data[0].strip())
    S = data[1].strip()
    n = len(S)
    BIG = K + 1
    
    for L in range(n, 0, -1):
        if L == 1:
            print(n - 1)
            return
        else:
            dp_prev = [BIG] * n
            for i in range(n):
                if S[i] == '1':
                    dp_prev[i] = 1
                    
            for j in range(2, L + 1):
                dp_curr = [BIG] * n
                min_prev = BIG
                for i in range(n):
                    if min_prev != BIG:
                        bit_val = 1 if S[i] == '1' else 0
                        candidate = min_prev * 2 + bit_val
                        if candidate <= K:
                            dp_curr[i] = candidate
                    if dp_prev[i] != BIG and dp_prev[i] < min_prev:
                        min_prev = dp_prev[i]
                
                if all(x == BIG for x in dp_curr):
                    break
                dp_prev = dp_curr
            else:
                for value in dp_prev:
                    if value <= K:
                        print(n - L)
                        return
    print(n - 1)

if __name__ == "__main__":
    main()