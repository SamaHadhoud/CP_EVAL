import bisect
import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    K = int(data[1])
    T = int(data[2])
    A = list(map(int, data[3:3+n]))
    
    if T == 0:
        print(0 if K == n else -1)
        return
            
    if K == n:
        print(0)
        return
        
    A.sort()
    P = [0] * (n+1)
    for i in range(1, n+1):
        P[i] = P[i-1] + A[i-1]
    
    candidates = set()
    candidates.add(0)
    if T-1 >= 0:
        candidates.add(T-1)
    for a in A:
        if 0 <= a <= T-1:
            candidates.add(a)
        if 0 <= a-1 <= T-1:
            candidates.add(a-1)
            
    best = float('inf')
    
    for m in candidates:
        r = bisect.bisect_right(A, m)
        if r >= K:
            cost_val = (m + 1) * (r - K) - P[r - K]
        else:
            s = K - r
            cost_val = (P[r + s] - P[r]) - m * s
        
        if cost_val < best:
            best = cost_val
            
    print(best if best != float('inf') else -1)

if __name__ == '__main__':
    main()