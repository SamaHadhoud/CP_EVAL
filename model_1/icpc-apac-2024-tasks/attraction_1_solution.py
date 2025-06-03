import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        print(0)
        return
        
    it = iter(data)
    n = int(next(it)); m = int(next(it))
    cities = []
    for i in range(n):
        x = int(next(it)); y = int(next(it))
        cities.append((x, y))
    
    graph = [[] for _ in range(n+1)]
    deg = [0] * (n+1)
    edges_dict = {}
    adj_set = [set() for _ in range(n+1)]
    
    for i in range(m):
        u = int(next(it)); v = int(next(it)); a = int(next(it))
        if u > v:
            u, v = v, u
        edges_dict[(u, v)] = a
        graph[u].append(v)
        graph[v].append(u)
        deg[u] += 1
        deg[v] += 1
        adj_set[u].add(v)
        adj_set[v].add(u)
        
    best = 0
    
    for key in edges_dict:
        a_val = edges_dict[key]
        if a_val > best:
            best = a_val
            
    triangles = []
    for (u, v) in edges_dict:
        common = adj_set[u] & adj_set[v]
        for w in common:
            if w > v:
                try:
                    a1 = edges_dict[(u, v)]
                    a2 = edges_dict[(u, w)]
                    a3 = edges_dict[(v, w)]
                except KeyError:
                    continue
                total_tri = a1 + a2 + a3
                if total_tri > best:
                    best = total_tri
                triangles.append((u, v, w))
                
    done_k4 = set()
    for (u, v, w) in triangles:
        common = adj_set[u] & adj_set[v] & adj_set[w]
        for x in common:
            if x == u or x == v or x == w:
                continue
            four = tuple(sorted([u, v, w, x]))
            if four in done_k4:
                continue
            done_k4.add(four)
            total_k4 = 0
            nodes = four
            valid = True
            for i in range(4):
                for j in range(i+1, 4):
                    a_val = min(nodes[i], nodes[j])
                    b_val = max(nodes[i], nodes[j])
                    if (a_val, b_val) in edges_dict:
                        total_k4 += edges_dict[(a_val, b_val)]
                    else:
                        valid = False
                        break
                if not valid:
                    break
            if valid and total_k4 > best:
                best = total_k4
                
    print(best)

if __name__ == '__main__':
    main()