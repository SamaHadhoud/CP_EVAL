import sys
import heapq

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    C_day = int(next(it))
    
    graph = [[] for _ in range(n + 1)]
    for _ in range(m):
        a = int(next(it))
        b = int(next(it))
        T = int(next(it))
        L = int(next(it))
        R = int(next(it))
        graph[a].append((b, T, L, R))
        graph[b].append((a, T, L, R))
    
    INF = 10**18
    dist = [INF] * (n + 1)
    dist[1] = 0
    heap = [(0, 1)]
    
    while heap:
        t, u = heapq.heappop(heap)
        if t != dist[u]:
            continue
        for edge in graph[u]:
            v, T, L, R = edge
            rem = t % C_day
            if rem <= R:
                wait_time = max(0, L - rem)
            else:
                wait_time = C_day - rem + L
            new_time = t + wait_time + T
            if new_time < dist[v]:
                dist[v] = new_time
                heapq.heappush(heap, (new_time, v))
                
    print(dist[n])

if __name__ == '__main__':
    main()