import heapq
import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    m = int(data[1])
    graph = [[] for _ in range(n+1)]
    edges = []
    index = 2
    for i in range(m):
        u = int(data[index])
        v = int(data[index+1])
        t = int(data[index+2])
        index += 3
        edges.append((u, v, t))
        graph[u].append((v, t))
        graph[v].append((u, t))
    
    INF = 10**18
    d1 = [INF] * (n+1)
    parent_arr = [0] * (n+1)
    heap = []
    d1[1] = 0
    heapq.heappush(heap, (0, 1))
    
    while heap:
        dist, u = heapq.heappop(heap)
        if dist != d1[u]:
            continue
        for (v, w) in graph[u]:
            nd = dist + w
            if nd < d1[v]:
                d1[v] = nd
                parent_arr[v] = u
                heapq.heappush(heap, (nd, v))
    
    T = d1[n]
    if T == INF:
        print(-1)
        return

    dn = [INF] * (n+1)
    heap = []
    dn[n] = 0
    heapq.heappush(heap, (0, n))
    while heap:
        dist, u = heapq.heappop(heap)
        if dist != dn[u]:
            continue
        for (v, w) in graph[u]:
            nd = dist + w
            if nd < dn[v]:
                dn[v] = nd
                heapq.heappush(heap, (nd, v))
    
    dist0 = [INF] * (n+1)
    dist1 = [INF] * (n+1)
    heap = []
    dist0[1] = 0
    heapq.heappush(heap, (0, 1))
    
    while heap:
        d, u = heapq.heappop(heap)
        if d > dist1[u]:
            continue
        for (v, w) in graph[u]:
            nd = d + w
            if nd < dist0[v]:
                dist1[v] = dist0[v]
                dist0[v] = nd
                heapq.heappush(heap, (nd, v))
            elif nd != dist0[v] and nd < dist1[v]:
                dist1[v] = nd
                heapq.heappush(heap, (nd, v))
    
    base_nodes = set()
    base_edges = set()
    cur = n
    base_nodes.add(n)
    while cur != 1:
        prev = parent_arr[cur]
        base_nodes.add(prev)
        u = min(cur, prev)
        v = max(cur, prev)
        base_edges.add((u, v))
        cur = prev
    
    candidate_A = INF
    if dist1[n] < INF:
        candidate_A = dist0[n] + dist1[n]
    
    candidate_B1 = INF
    candidate_B4 = INF
    
    for (u, v, t) in edges:
        a, b = min(u, v), max(u, v)
        if (a, b) in base_edges:
            continue
        
        if u in base_nodes and v in base_nodes:
            seg = abs(d1[u] - d1[v])
            cand = 2 * T - seg + t
            if cand < candidate_B1:
                candidate_B1 = cand
        else:
            cand1 = T + d1[u] + t + dn[v]
            cand2 = T + d1[v] + t + dn[u]
            cand = min(cand1, cand2)
            if cand < candidate_B4:
                candidate_B4 = cand
                
    ans = min(candidate_A, candidate_B1, candidate_B4)
    if ans >= INF:
        print(-1)
    else:
        print(ans)

if __name__ == '__main__':
    main()