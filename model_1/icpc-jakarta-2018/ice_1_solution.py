import sys
from collections import deque

def main():
    data = sys.stdin.read().splitlines()
    if not data:
        return
    R, C = map(int, data[0].split())
    grid = []
    for i in range(1, 1+R):
        grid.append(data[i].strip())
    
    rowEmpty = [True] * R
    colEmpty = [True] * C

    for i in range(R):
        for j in range(C):
            if grid[i][j] == '#':
                rowEmpty[i] = False
                colEmpty[j] = False

    emptyRows = [i for i in range(R) if rowEmpty[i]]
    emptyCols = [j for j in range(C) if colEmpty[j]]
    
    A = len(emptyRows)
    B = len(emptyCols)
    
    if A == 0 and B == 0:
        print(0)
        return
        
    graph0 = [[] for _ in range(A)]
    for i in range(A):
        r = emptyRows[i]
        for j in range(B):
            c = emptyCols[j]
            if grid[r][c] == '.':
                graph0[i].append(j)
    
    if A == 0 or B == 0:
        print(A + B)
        return

    matchL = [-1] * A
    matchR = [-1] * B
    dist = [-1] * A
    matching_count = 0

    def bfs():
        nonlocal dist
        queue = deque()
        for i in range(A):
            if matchL[i] == -1:
                dist[i] = 0
                queue.append(i)
            else:
                dist[i] = -1
        found = False
        while queue:
            u = queue.popleft()
            for v in graph0[u]:
                u_next = matchR[v]
                if u_next == -1:
                    found = True
                elif u_next != -1 and dist[u_next] == -1:
                    dist[u_next] = dist[u] + 1
                    queue.append(u_next)
        return found

    def dfs(u):
        for v in graph0[u]:
            u_next = matchR[v]
            if u_next == -1:
                matchR[v] = u
                matchL[u] = v
                return True
            if dist[u_next] == dist[u] + 1:
                if dfs(u_next):
                    matchR[v] = u
                    matchL[u] = v
                    return True
        return False

    while bfs():
        for u in range(A):
            if matchL[u] == -1:
                if dfs(u):
                    matching_count += 1
                    
    ans = A + B - matching_count
    print(ans)

if __name__ == "__main__":
    main()