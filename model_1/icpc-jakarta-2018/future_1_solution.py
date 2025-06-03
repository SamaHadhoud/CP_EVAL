import sys

def main():
    data = sys.stdin.read().splitlines()
    if not data:
        return
    N = int(data[0].strip())
    S = []
    for i in range(1, 1 + N):
        S.append(data[i].strip())
    
    A_list = []
    for s in S:
        n = len(s)
        seen = set()
        for mask in range(1, 1 << n):
            substr = []
            for j in range(n):
                if mask & (1 << j):
                    substr.append(s[j])
            t = ''.join(substr)
            seen.add(t)
        sorted_seen = sorted(seen)
        A_list.append(sorted_seen)
    
    if N == 0:
        print(0)
        return
        
    dp_prev = [len(subseq) for subseq in A_list[0]]
    
    for i in range(1, N):
        prev_A = A_list[i-1]
        current_A = A_list[i]
        j = 0
        best_so_far = -10**9
        current_dp = [-10**9] * len(current_A)
        
        for idx in range(len(current_A)):
            s = current_A[idx]
            while j < len(prev_A) and prev_A[j] < s:
                if dp_prev[j] > best_so_far:
                    best_so_far = dp_prev[j]
                j += 1
            
            if best_so_far != -10**9:
                current_dp[idx] = best_so_far + len(s)
        
        dp_prev = current_dp
        
    ans = max(dp_prev) if dp_prev else -10**9
    if ans < 0:
        print(-1)
    else:
        print(ans)

if __name__ == "__main__":
    main()