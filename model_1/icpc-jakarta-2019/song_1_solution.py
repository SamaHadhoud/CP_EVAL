import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    L = int(data[1])
    R = int(data[2])
    K = int(data[3])
    A = list(map(int, data[4:4+n]))
    
    low = [0] * n
    high = [0] * n
    low[-1] = L
    high[-1] = R
    
    for i in range(n-2, -1, -1):
        if A[i] < A[i+1]:
            low_i_val = max(L, low[i+1] - K)
            high_i_val = min(R, high[i+1] - 1)
        elif A[i] == A[i+1]:
            low_i_val = max(L, low[i+1])
            high_i_val = min(R, high[i+1])
        else:
            low_i_val = max(L, low[i+1] + 1)
            high_i_val = min(R, high[i+1] + K)
            
        if low_i_val > high_i_val:
            print(-1)
            return
        else:
            low[i] = low_i_val
            high[i] = high_i_val
            
    res = [0] * n
    res[0] = low[0]
    
    for i in range(0, n-1):
        if A[i] < A[i+1]:
            next_val = res[i] + 1
            res[i+1] = max(low[i+1], next_val)
        elif A[i] == A[i+1]:
            res[i+1] = res[i]
        else:
            next_val = res[i] - K
            res[i+1] = max(low[i+1], next_val)
            
    print(" ".join(map(str, res)))

if __name__ == "__main__":
    main()