import sys
from collections import deque

mod1 = 10**9 + 7
mod2 = 10**9 + 9
base = 131

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    graph = [[] for _ in range(n)]
    index = 1
    for i in range(n-1):
        a = int(data[index]); b = int(data[index+1]); index += 2
        a -= 1; b -= 1
        graph[a].append(b)
        graph[b].append(a)
    
    deg = [len(graph[i]) for i in range(n)]
    ans = -1

    for u in range(n):
        k = deg[u]
        if k < 2:
            continue
        total = n - 1
        if total % k != 0:
            continue
        s = total // k

        visited = [False] * n
        visited[u] = True
        comps = []
        for v in graph[u]:
            if not visited[v]:
                comp_set = set()
                queue = deque([v])
                visited[v] = True
                comp_set.add(v)
                while queue:
                    node = queue.popleft()
                    for neighbor in graph[node]:
                        if not visited[neighbor] and neighbor != u:
                            visited[neighbor] = True
                            comp_set.add(neighbor)
                            queue.append(neighbor)
                comps.append(comp_set)
                
        if len(comps) != k:
            continue
            
        valid_size = True
        for comp_set in comps:
            if len(comp_set) != s:
                valid_size = False
                break
        if not valid_size:
            continue

        comp_hashes = []
        for comp_set in comps:
            n_nodes = len(comp_set)
            if n_nodes == 0:
                continue
            deg_comp = [0] * n
            for node in comp_set:
                for neighbor in graph[node]:
                    if neighbor in comp_set:
                        deg_comp[node] += 1

            if n_nodes == 1:
                centers = [next(iter(comp_set))]
            else:
                leaves = deque()
                deg_temp = deg_comp[:]
                for node in comp_set:
                    if deg_temp[node] == 1:
                        leaves.append(node)
                current_nodes = n_nodes
                while current_nodes > 2:
                    num_leaves = len(leaves)
                    current_nodes -= num_leaves
                    new_leaves = deque()
                    for _ in range(num_leaves):
                        node = leaves.popleft()
                        for neighbor in graph[node]:
                            if neighbor in comp_set and deg_temp[neighbor] > 1:
                                deg_temp[neighbor] -= 1
                                if deg_temp[neighbor] == 1:
                                    new_leaves.append(neighbor)
                    leaves = new_leaves
                centers = list(leaves)
            
            center_hashes = []
            for center in centers:
                parent_arr = [-1] * n
                children_arr = [[] for _ in range(n)]
                order = []
                q_bfs = deque([center])
                parent_arr[center] = -1
                while q_bfs:
                    node = q_bfs.popleft()
                    order.append(node)
                    for neighbor in graph[node]:
                        if neighbor in comp_set and neighbor != parent_arr[node]:
                            parent_arr[neighbor] = node
                            children_arr[node].append(neighbor)
                            q_bfs.append(neighbor)
                
                h1 = [1] * n
                h2 = [1] * n
                for node in reversed(order):
                    child_hashes = []
                    for child in children_arr[node]:
                        child_hashes.append((h1[child], h2[child]))
                    child_hashes.sort(key=lambda x: (x[0], x[1]))
                    h_val1 = 1
                    h_val2 = 1
                    for a, b in child_hashes:
                        h_val1 = (h_val1 * base + a) % mod1
                        h_val2 = (h_val2 * base + b) % mod2
                    h1[node] = h_val1
                    h2[node] = h_val2
                center_hashes.append((h1[center], h2[center]))
            
            if not center_hashes:
                comp_hash = (0, 0)
            else:
                comp_hash = min(center_hashes)
            comp_hashes.append(comp_hash)
        
        if all(hash_val == comp_hashes[0] for hash_val in comp_hashes):
            if k > ans:
                ans = k

    print(ans if ans != -1 else -1)

if __name__ == '__main__':
    main()