import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it))
    graph = [[] for _ in range(n+1)]
    for _ in range(n-1):
        a = int(next(it))
        b = int(next(it))
        graph[a].append(b)
        graph[b].append(a)
    
    depth = [0] * (n+1)
    parent0 = [0] * (n+1)
    graph[0] = []
    
    queue = deque([1])
    depth[1] = 0
    parent0[1] = 0
    while queue:
        u = queue.popleft()
        for v in graph[u]:
            if v == parent0[u]:
                continue
            depth[v] = depth[u] + 1
            parent0[v] = u
            queue.append(v)
    
    max_log = 0
    temp = 1
    while temp <= n:
        max_log += 1
        temp *= 2
    max_log -= 1
    if max_log < 0:
        max_log = 0
    
    parent_table = [[0] * (n+1) for _ in range(max_log+1)]
    for i in range(1, n+1):
        parent_table[0][i] = parent0[i]
    
    for i in range(1, max_log+1):
        for node in range(1, n+1):
            prev = parent_table[i-1][node]
            parent_table[i][node] = parent_table[i-1][prev] if prev != 0 else 0
    
    def lca(u, v):
        if depth[u] < depth[v]:
            u, v = v, u
        diff = depth[u] - depth[v]
        bit = 0
        while diff:
            if diff & 1:
                u = parent_table[bit][u]
            bit += 1
            diff //= 2
        
        if u == v:
            return u
        
        for i in range(max_log, -1, -1):
            if parent_table[i][u] != parent_table[i][v]:
                u = parent_table[i][u]
                v = parent_table[i][v]
        return parent_table[0][u]
    
    def dist(u, v):
        w = lca(u, v)
        return depth[u] + depth[v] - 2 * depth[w]
    
    def climb(u, steps):
        node = u
        for i in range(max_log+1):
            if steps & (1 << i):
                node = parent_table[i][node]
                if node == 0:
                    break
        return node
    
    q = int(next(it))
    out_lines = []
    for _ in range(q):
        k = int(next(it))
        robots = []
        for _ in range(k):
            robots.append(int(next(it)))
        
        if k == 1:
            out_lines.append("0")
            continue
        
        u0 = robots[0]
        best_dist = -1
        candidate_u = u0
        for s in robots:
            d_val = dist(u0, s)
            if d_val > best_dist:
                best_dist = d_val
                candidate_u = s
        u = candidate_u
        
        best_dist = -1
        candidate_v = u0
        for s in robots:
            d_val = dist(u, s)
            if d_val > best_dist:
                best_dist = d_val
                candidate_v = s
        v = candidate_v
        
        w = lca(u, v)
        d1 = depth[u] - depth[w]
        d2 = depth[v] - depth[w]
        d_total = d1 + d2
        
        def get_node_at_step(t):
            if t <= d1:
                return climb(u, t)
            else:
                return climb(v, d_total - t)
        
        def g(t):
            x = get_node_at_step(t)
            total = 0
            for s in robots:
                total += dist(s, x)
            return total
        
        low = 0
        high = d_total
        while high - low > 2:
            d_len = high - low
            mid1 = low + d_len // 3
            mid2 = high - d_len // 3
            f1 = g(mid1)
            f2 = g(mid2)
            if f1 < f2:
                high = mid2
            else:
                low = mid1
        
        best_val = 10**18
        for t in range(low, high+1):
            val = g(t)
            if val < best_val:
                best_val = val
        out_lines.append(str(best_val))
    
    sys.stdout.write("\n".join(out_lines))

if __name__ == '__main__':
    main()