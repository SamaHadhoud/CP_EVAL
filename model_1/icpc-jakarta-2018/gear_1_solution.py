import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    it = iter(data)
    n = int(next(it))
    adj = [[] for _ in range(n+1)]
    for _ in range(n-1):
        u = int(next(it))
        v = int(next(it))
        adj[u].append(v)
        adj[v].append(u)
    
    depth = [-1] * (n+1)
    sign_arr = [0] * (n+1)
    depth[1] = 0
    queue = deque([1])
    while queue:
        u = queue.popleft()
        for v in adj[u]:
            if depth[v] == -1:
                depth[v] = depth[u] + 1
                queue.append(v)
                
    for i in range(1, n+1):
        if depth[i] % 2 == 0:
            sign_arr[i] = 1
        else:
            sign_arr[i] = -1
            
    q_count = int(next(it))
    operations = []
    for _ in range(q_count):
        op_type = next(it)
        if op_type == '1' or op_type == '2':
            x = int(next(it))
            operations.append((op_type, x))
        else:
            x = int(next(it))
            alpha = int(next(it))
            operations.append((op_type, x, alpha))
            
    active = [True] * (n+1)
    comp_id = [0] * (n+1)
    for i in range(1, n+1):
        comp_id[i] = 1
        
    rotation = [0] * (n+1)
    next_comp_id = 2
    
    output_lines = []
    
    for op in operations:
        if op[0] == '1':
            x = op[1]
            active[x] = False
            old_cid = comp_id[x]
            for neighbor in adj[x]:
                if active[neighbor] and comp_id[neighbor] == old_cid:
                    new_cid = next_comp_id
                    next_comp_id += 1
                    stack = [neighbor]
                    comp_id[neighbor] = new_cid
                    while stack:
                        u = stack.pop()
                        for v in adj[u]:
                            if v == x or not active[v] or comp_id[v] != old_cid:
                                continue
                            comp_id[v] = new_cid
                            stack.append(v)
        elif op[0] == '2':
            x = op[1]
            active[x] = True
            new_cid = next_comp_id
            next_comp_id += 1
            comp_id[x] = new_cid
            for neighbor in adj[x]:
                if active[neighbor]:
                    if comp_id[neighbor] != new_cid:
                        old_cid = comp_id[neighbor]
                        stack = [neighbor]
                        comp_id[neighbor] = new_cid
                        while stack:
                            u = stack.pop()
                            for v in adj[u]:
                                if v == x or not active[v] or comp_id[v] != old_cid:
                                    continue
                                comp_id[v] = new_cid
                                stack.append(v)
        else:
            x = op[1]
            alpha = op[2]
            cid = comp_id[x]
            stack = [x]
            visited = set([x])
            count = 0
            while stack:
                u = stack.pop()
                count += 1
                rotation[u] += sign_arr[x] * sign_arr[u] * alpha
                for v in adj[u]:
                    if active[v] and comp_id[v] == cid and v not in visited:
                        visited.add(v)
                        stack.append(v)
            energy = count * alpha
            output_lines.append(str(energy))
            
    total_deg_sum = 0
    for i in range(1, n+1):
        deg = rotation[i] % 360
        if deg < 0:
            deg = (deg % 360 + 360) % 360
        total_deg_sum += deg
        
    output_lines.append(str(total_deg_sum))
    sys.stdout.write("\n".join(output_lines))

if __name__ == "__main__":
    main()