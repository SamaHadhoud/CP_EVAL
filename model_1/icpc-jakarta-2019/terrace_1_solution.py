import sys

def main():
    data = sys.stdin.read().splitlines()
    if not data:
        return
    first_line = data[0].split()
    n = int(first_line[0])
    K = int(first_line[1])
    G1 = int(first_line[2])
    G2 = int(first_line[3])
    G3 = int(first_line[4])
    s = data[1].strip()
    
    total_soil = s.count('.')
    rocks = []
    for i, char in enumerate(s):
        if char == '#':
            rocks.append(i)
    m = len(rocks)
    
    if m == 0:
        ans = 0
        rem = n
        for x in range(0, min(K, rem) + 1):
            y = (rem - x) // 2
            candidate = x * G1 + y * G2
            if candidate > ans:
                ans = candidate
        print(ans)
        return

    segs = [0] * (m + 1)
    segs[0] = rocks[0]
    for i in range(1, m):
        segs[i] = rocks[i] - rocks[i-1] - 1
    segs[m] = n - 1 - rocks[m-1]
    
    eligible = [False] * m
    for i in range(m):
        if rocks[i] > 0 and rocks[i] < n - 1 and s[rocks[i]-1] == '.' and s[rocks[i]+1] == '.':
            eligible[i] = True

    T_max = 0
    if m > 0:
        dp0 = [0] * m
        dp1 = [-10**9] * m
        if eligible[0]:
            dp1[0] = 1
        else:
            dp1[0] = -10**9
        dp0[0] = 0
        
        for i in range(1, m):
            dp0[i] = max(dp0[i-1], dp1[i-1])
            if eligible[i]:
                candidate = 1 + dp0[i-1]
                if rocks[i] - rocks[i-1] > 2:
                    candidate = max(candidate, 1 + dp1[i-1])
                dp1[i] = candidate
            else:
                dp1[i] = -10**9
        T_max = max(dp0[m-1], dp1[m-1])
    else:
        T_max = 0
    
    if T_max < 0:
        T_max = 0
    
    ans = 0
    for T in range(0, T_max + 1):
        rem = total_soil - 2 * T
        if rem < 0:
            continue
        best_soil = 0
        up = min(K, rem)
        for x in range(0, up + 1):
            y = (rem - x) // 2
            candidate_soil = x * G1 + y * G2
            if candidate_soil > best_soil:
                best_soil = candidate_soil
        total_candidate = T * G3 + best_soil
        if total_candidate > ans:
            ans = total_candidate
    print(ans)

if __name__ == '__main__':
    main()