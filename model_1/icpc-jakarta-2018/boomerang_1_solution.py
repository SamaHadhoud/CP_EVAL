import collections

def main():
    import sys
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    m = int(data[1])
    g = [set() for _ in range(n+1)]
    index = 2
    for i in range(m):
        u = int(data[index])
        v = int(data[index+1])
        index += 2
        g[u].add(v)
        g[v].add(u)
    
    in_queue = [False] * (n+1)
    q = collections.deque()
    
    for i in range(1, n+1):
        if len(g[i]) >= 2:
            in_queue[i] = True
            q.append(i)
            
    ans = []
    
    while q:
        v = q.popleft()
        in_queue[v] = False
        
        if len(g[v]) < 2:
            continue
            
        a = g[v].pop()
        g[a].discard(v)
        
        if not g[v]:
            g[a].add(v)
            g[v].add(a)
            continue
            
        b = g[v].pop()
        g[b].discard(v)
        
        ans.append((a, v, b))
        
        for u in [v, a, b]:
            if len(g[u]) >= 2 and not in_queue[u]:
                in_queue[u] = True
                q.append(u)
                
    print(len(ans))
    for (a, v, b) in ans:
        print(f"{a} {v} {b}")

if __name__ == '__main__':
    main()