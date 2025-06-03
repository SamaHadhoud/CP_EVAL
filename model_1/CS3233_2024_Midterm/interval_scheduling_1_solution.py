def main():
    import sys
    data = sys.stdin.read().splitlines()
    n = int(data[0].strip())
    intervals = []
    for i in range(1, n + 1):
        line = data[i].strip()
        parts = line.split(' - ')
        start_str = parts[0].strip()
        end_str = parts[1].strip()
        intervals.append((start_str, end_str))
    
    def to_minutes(time_str):
        h, m = time_str.split(':')
        return int(h) * 60 + int(m)
    
    def to_time(minutes):
        h = minutes // 60
        m = minutes % 60
        return f"{h:02d}:{m:02d}"
    
    starts = []
    ends = []
    for start, end in intervals:
        starts.append(to_minutes(start))
        ends.append(to_minutes(end))
    
    first_start = starts[0]
    last_end = ends[-1]
    
    def feasible(L):
        S = [0] * n
        E = [0] * n
        S[-1] = min(starts[-1], last_end - L)
        E[-1] = max(ends[-1], S[-1] + L)
        if E[-1] != last_end:
            return False
        
        for i in range(n-2, -1, -1):
            S[i] = min(starts[i], S[i+1] - L)
            E[i] = max(ends[i], S[i] + L)
            if E[i] > S[i+1]:
                return False
        
        if S[0] != first_start:
            return False
        
        return True
    
    low = 0
    high = last_end - first_start
    ans_L = 0
    while low <= high:
        mid = (low + high) // 2
        if feasible(mid):
            ans_L = mid
            low = mid + 1
        else:
            high = mid - 1
    
    S_opt = [0] * n
    E_opt = [0] * n
    S_opt[-1] = min(starts[-1], last_end - ans_L)
    E_opt[-1] = max(ends[-1], S_opt[-1] + ans_L)
    for i in range(n-2, -1, -1):
        S_opt[i] = min(starts[i], S_opt[i+1] - ans_L)
        E_opt[i] = max(ends[i], S_opt[i] + ans_L)
    
    output_lines = []
    for i in range(n):
        start_time = to_time(S_opt[i])
        end_time = to_time(E_opt[i])
        output_lines.append(f"{start_time} - {end_time}")
    
    for line in output_lines:
        print(line)

if __name__ == '__main__':
    main()