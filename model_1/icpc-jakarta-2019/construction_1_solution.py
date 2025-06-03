import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    it = iter(data)
    n = int(next(it)); k = int(next(it))
    
    A = [0] * (n+1)
    M_list = [0] * (n+1)
    B_list = [None] * (n+1)
    for i in range(1, n+1):
        A_i = int(next(it)); M_i = int(next(it))
        A[i] = A_i
        M_list[i] = M_i
        B_i = []
        for j in range(M_i):
            B_i.append(int(next(it)))
        B_list[i] = B_i
        
    workers = []
    for i in range(k):
        c = int(next(it))
        workers.append(c)
    
    graph = [[] for _ in range(n+1)]
    for i in range(1, n+1):
        u = i
        v = A[i]
        graph[u].append(v)
        graph[v].append(u)
    
    deg = [len(graph[i]) for i in range(n+1)]
    q = deque()
    for i in range(1, n+1):
        if deg[i] == 1:
            q.append(i)
    
    non_cycle_edges_set = set()
    temp_deg = deg[:]
    while q:
        u = q.popleft()
        if temp_deg[u] <= 0:
            continue
        for v in graph[u]:
            if temp_deg[v] <= 0:
                continue
            key = (min(u, v), max(u, v))
            if key in non_cycle_edges_set:
                continue
            non_cycle_edges_set.add(key)
            temp_deg[u] -= 1
            temp_deg[v] -= 1
            if temp_deg[v] == 1:
                q.append(v)
    
    cycle_edges_set = set()
    original_rep = {}
    materials_dict = {}
    for i in range(1, n+1):
        u = i
        v = A[i]
        key = (min(u, v), max(u, v))
        original_rep[key] = (u, v)
        materials_dict[key] = B_list[i]
        if key not in non_cycle_edges_set:
            cycle_edges_set.add(key)
    
    non_cycle_edges_list = list(non_cycle_edges_set)
    cycle_edges_list = list(cycle_edges_set)
    L = len(cycle_edges_list)
    
    if k < n - 1:
        print(-1)
        return
        
    material_to_workers = {}
    for worker_id, material in enumerate(workers):
        if material not in material_to_workers:
            material_to_workers[material] = []
        material_to_workers[material].append(worker_id)
    
    worker_list_by_key = {}
    all_edges = non_cycle_edges_list + cycle_edges_list
    for key in all_edges:
        materials = materials_dict[key]
        worker_list = []
        for m in materials:
            if m in material_to_workers:
                for w in material_to_workers[m]:
                    worker_list.append(w)
        worker_list_by_key[key] = worker_list

    for key in non_cycle_edges_list:
        worker_list = worker_list_by_key[key]
        worker_list.sort()
    
    for key in cycle_edges_list:
        worker_list = worker_list_by_key[key]
        worker_list.sort(reverse=True)
    
    match_non = [-1] * len(non_cycle_edges_list)
    match_worker = [-1] * k
    
    def dfs_non(i, seen):
        key = non_cycle_edges_list[i]
        worker_list = worker_list_by_key[key]
        for worker_id in worker_list:
            if not seen[worker_id]:
                seen[worker_id] = True
                if match_worker[worker_id] == -1 or dfs_non(match_worker[worker_id], seen):
                    match_worker[worker_id] = i
                    match_non[i] = worker_id
                    return True
        return False

    for idx in range(len(non_cycle_edges_list)):
        seen = [False] * k
        if not dfs_non(idx, seen):
            print(-1)
            return
            
    match_cycle = [-1] * len(cycle_edges_list)
    match_worker_cycle = [-1] * k

    def dfs_cycle(i, seen):
        key = cycle_edges_list[i]
        worker_list = worker_list_by_key[key]
        for worker_id in worker_list:
            if match_worker[worker_id] != -1:
                continue
            if not seen[worker_id]:
                seen[worker_id] = True
                if match_worker_cycle[worker_id] == -1 or dfs_cycle(match_worker_cycle[worker_id], seen):
                    match_worker_cycle[worker_id] = i
                    match_cycle[i] = worker_id
                    return True
        return False

    M_cycle = 0
    for idx in range(len(cycle_edges_list)):
        seen = [False] * k
        if dfs_cycle(idx, seen):
            M_cycle += 1
            
    if M_cycle < L - 1:
        print(-1)
        return
        
    res = ['0 0'] * k
    for i, key in enumerate(non_cycle_edges_list):
        worker_id = match_non[i]
        if worker_id != -1:
            u, v = original_rep[key]
            res[worker_id] = f"{u} {v}"
            
    for i, key in enumerate(cycle_edges_list):
        worker_id = match_cycle[i]
        if worker_id != -1:
            u, v = original_rep[key]
            res[worker_id] = f"{u} {v}"
            
    for i in range(k):
        print(res[i])

if __name__ == "__main__":
    main()