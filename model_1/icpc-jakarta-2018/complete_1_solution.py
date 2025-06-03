import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    m = int(data[1])
    graph = [[False] * (n+1) for _ in range(n+1)]
    deg = [0] * (n+1)
    idx = 2
    for _ in range(m):
        u = int(data[idx])
        v = int(data[idx+1])
        idx += 2
        graph[u][v] = True
        graph[v][u] = True
        deg[u] += 1
        deg[v] += 1

    total_edges = n * (n - 1) // 2
    target_edges_to_add = total_edges - m
    if target_edges_to_add == 0:
        print(0)
        return

    def check(k):
        if k == 0:
            return True
        g = [row[:] for row in graph]
        deg_copy = deg[:]
        q = deque()
        in_queue = set()
        
        for i in range(1, n+1):
            for j in range(i+1, n+1):
                if not g[i][j] and deg_copy[i] + deg_copy[j] >= k:
                    in_queue.add((i, j))
                    q.append((i, j))
                    
        count_added = 0
        while q and count_added < target_edges_to_add:
            i, j = q.popleft()
            in_queue.discard((i, j))
            
            if g[i][j]:
                continue
            if deg_copy[i] + deg_copy[j] < k:
                continue
                
            g[i][j] = True
            g[j][i] = True
            count_added += 1
            deg_copy[i] += 1
            deg_copy[j] += 1
            
            for node in (i, j):
                for neighbor in range(1, n+1):
                    if neighbor == node:
                        continue
                    if not g[node][neighbor]:
                        a, b = min(node, neighbor), max(node, neighbor)
                        if (a, b) not in in_queue:
                            s = deg_copy[node] + deg_copy[neighbor]
                            if s >= k:
                                in_queue.add((a, b))
                                q.append((a, b))
        return count_added == target_edges_to_add

    low = 0
    high = 2 * (n - 1)
    ans = 0
    while low <= high:
        mid = (low + high) // 2
        if check(mid):
            ans = mid
            low = mid + 1
        else:
            high = mid - 1
    print(ans)

if __name__ == '__main__':
    main()