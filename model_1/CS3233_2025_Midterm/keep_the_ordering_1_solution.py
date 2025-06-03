import collections

def main():
    max_val = 10**18
    q = collections.deque()
    valid_n = []
    
    for d in range(1, 10):
        q.append((d, d))
    
    while q:
        n, last = q.popleft()
        
        n_sq = n * n
        s = str(n_sq)
        flag = True
        for i in range(1, len(s)):
            if s[i] < s[i-1]:
                flag = False
                break
        if flag:
            valid_n.append(n)
        
        if n * 10 > max_val:
            continue
            
        for d in range(last, 10):
            next_num = n * 10 + d
            if next_num > max_val:
                break
            q.append((next_num, d))
    
    T = int(input().strip())
    out_lines = []
    for _ in range(T):
        X = int(input().strip())
        cnt = 0
        for num in valid_n:
            if num <= X:
                cnt += 1
            else:
                break
        out_lines.append(str(cnt))
    
    print("\n".join(out_lines))

if __name__ == '__main__':
    main()