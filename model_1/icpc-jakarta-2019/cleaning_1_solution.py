MOD = 10**9 + 7

import sys
sys.setrecursionlimit(300000)

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    graph = [[] for _ in range(n)]
    index = 1
    for _ in range(n - 1):
        u = int(data[index])
        v = int(data[index + 1])
        index += 2
        u -= 1
        v -= 1
        graph[u].append(v)
        graph[v].append(u)
    
    parent = [-1] * n
    children = [[] for _ in range(n)]
    stack = [0]
    while stack:
        u = stack.pop()
        for v in graph[u]:
            if v == parent[u]:
                continue
            parent[v] = u
            children[u].append(v)
            stack.append(v)
    
    order = []
    stack = [0]
    while stack:
        u = stack.pop()
        order.append(u)
        for v in children[u]:
            stack.append(v)
    order.reverse()
    
    dp0 = [0] * n
    dp1 = [0] * n
    dp2 = [0] * n
    
    for u in order:
        dp0[u] = 1
        dp1[u] = 0
        dp2[u] = 0
        for v in children[u]:
            old0 = dp0[u]
            old1 = dp1[u]
            old2 = dp2[u]
            c0 = dp0[v]
            c1 = dp1[v]
            c2 = dp2[v]
            
            nd0 = (2 * old0 * c0 + 2 * old0 * c1 + old0 * c2 +
                   old1 * c0 + old1 * c1 +
                   old2 * c0 + old2 * c1) % MOD
            nd1 = (old0 * (c0 + c1) +
                   old1 * c0) % MOD
            nd2 = (old0 * (c0 + c1) +
                   old2 * c0) % MOD
            
            dp0[u] = nd0
            dp1[u] = nd1
            dp2[u] = nd2
    
    ans = (dp0[0] + dp1[0]) % MOD
    print(ans)

if __name__ == '__main__':
    main()