import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    M_val = int(next(it))
    A = [int(next(it)) for _ in range(n)]
    B = [int(next(it)) for _ in range(n)]
    
    prefixA = [0]
    k = 0
    total = 0
    for i in range(n):
        total += A[i]
        if total <= M_val:
            prefixA.append(total)
            k = len(prefixA) - 1
        else:
            break
            
    if k == 0:
        print(-1)
        return
        
    s_arr = [0]
    for i in range(k):
        next_s = s_arr[-1] + B[i]
        s_arr.append(next_s)
        
    for i in range(k):
        if s_arr[i] + B[i] > M_val:
            print(0)
            return
            
    first_balloon = prefixA[1]
    best_t = float('inf')
    for i in range(k):
        t_val = max(s_arr[i], first_balloon, M_val - B[i] + 1)
        if t_val < best_t:
            best_t = t_val
            
    print(1)
    print(best_t)

if __name__ == '__main__':
    main()