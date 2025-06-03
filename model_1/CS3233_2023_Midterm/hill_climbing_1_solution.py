import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    t = int(data[0])
    index = 1
    out_lines = []
    
    for _ in range(t):
        n = int(data[index]); m = int(data[index+1]); index += 2
        a_list = list(map(int, data[index:index+m]))
        index += m
        edges = []
        for i in range(m):
            u = int(data[index]); v = int(data[index+1]); index += 2
            edges.append((u, v))
        
        T = (n + 1) // 2
        
        total_cap = sum(a_list)
        if total_cap < T:
            out_lines.append("No")
            continue
        
        cnt = 0
        for x in a_list:
            if x >= 1:
                cnt += 1
                
        if cnt >= T:
            non_zero_colors = []
            for i in range(m):
                if a_list[i] >= 1:
                    non_zero_colors.append(i+1)
            non_zero_colors = non_zero_colors[:T]
            res = [0] * m
            for i in range(T):
                res[i] = non_zero_colors[i]
            out_lines.append("Yes")
            out_lines.append(" ".join(map(str, res)))
        else:
            graph = [[] for _ in range(n+1)]
            for i in range(m):
                u, v = edges[i]
                graph[u].append((v, i))
                graph[v].append((u, i))
            
            colors_with_index = []
            for i in range(m):
                if a_list[i] > 0:
                    colors_with_index.append((a_list[i], i))
            colors_with_index.sort(key=lambda x: x[0], reverse=True)
            
            unused_edges = set(range(m))
            components = []
            
            for cap, color_idx in colors_with_index:
                if not unused_edges:
                    break
                start_edge = next(iter(unused_edges))
                comp_edges = set()
                comp_edges.add(start_edge)
                unused_edges.remove(start_edge)
                u0, v0 = edges[start_edge]
                vertices = set([u0, v0])
                qv = deque([u0, v0])
                
                while len(comp_edges) < cap and qv:
                    u = qv.popleft()
                    for neighbor, eidx in graph[u]:
                        if eidx in unused_edges:
                            comp_edges.add(eidx)
                            unused_edges.remove(eidx)
                            if neighbor not in vertices:
                                vertices.add(neighbor)
                                qv.append(neighbor)
                            if len(comp_edges) >= cap:
                                break
                components.append((color_idx, comp_edges))
            
            total_edges_colored = 0
            for _, comp_set in components:
                total_edges_colored += len(comp_set)
                
            if total_edges_colored >= T:
                res = [0] * m
                for color_idx, comp_set in components:
                    c_val = color_idx + 1
                    for eidx in comp_set:
                        res[eidx] = c_val
                out_lines.append("Yes")
                out_lines.append(" ".join(map(str, res)))
            else:
                out_lines.append("No")
                
    print("\n".join(out_lines))

if __name__ == '__main__':
    main()