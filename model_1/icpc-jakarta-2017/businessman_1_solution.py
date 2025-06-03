import sys
import heapq
import bisect

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    it = iter(data)
    n = int(next(it)); m = int(next(it))
    S = [0] * (n+1)
    for i in range(1, n+1):
        S[i] = int(next(it))
    
    graph = [[] for _ in range(n+1)]
    for _ in range(m):
        u = int(next(it)); v = int(next(it))
        graph[u].append(v)
        graph[v].append(u)
    
    color = [-1] * (n+1)
    visited = [False] * (n+1)
    is_bipartite = True
    q = []
    color[1] = 0
    visited[1] = True
    q.append(1)
    while q:
        u = q.pop(0)
        for v in graph[u]:
            if not visited[v]:
                visited[v] = True
                color[v] = color[u] ^ 1
                q.append(v)
            else:
                if color[v] == color[u]:
                    is_bipartite = False

    q_count = int(next(it))
    queries = []
    for _ in range(q_count):
        a = int(next(it)); b = int(next(it)); c = int(next(it))
        queries.append((a, b, c))
    
    if not is_bipartite:
        out_lines = []
        for (a, b, c) in queries:
            if a == 1:
                out_lines.append("0")
            else:
                pass
        print("\n".join(out_lines))
    else:
        sorted_list = []
        for i in range(1, n+1):
            sorted_list.append((S[i], i, color[i]))
        sorted_list.sort()
        
        heap = []
        invalid = set()
        
        for i in range(1, len(sorted_list)):
            left_val, left_node, left_color = sorted_list[i-1]
            right_val, right_node, right_color = sorted_list[i]
            if left_color != right_color:
                gap = right_val - left_val
                heapq.heappush(heap, (gap, left_node, right_node))
        
        current_value = S[:]
        out_lines = []
        
        for (a, b, c) in queries:
            if a == 0:
                u = b
                new_val = c
                old_val = current_value[u]
                old_tuple = (old_val, u, color[u])
                i = bisect.bisect_left(sorted_list, old_tuple)
                left_neighbor = None
                right_neighbor = None
                if i < len(sorted_list) and sorted_list[i] == old_tuple:
                    if i-1 >= 0:
                        left_neighbor = sorted_list[i-1]
                    if i+1 < len(sorted_list):
                        right_neighbor = sorted_list[i+1]
                    
                    if left_neighbor:
                        if left_neighbor[2] != old_tuple[2]:
                            key = (left_neighbor[1], old_tuple[1])
                            invalid.add(key)
                    if right_neighbor:
                        if old_tuple[2] != right_neighbor[2]:
                            key = (old_tuple[1], right_neighbor[1])
                            invalid.add(key)
                    
                    del sorted_list[i]
                    
                    if left_neighbor and right_neighbor:
                        if left_neighbor[2] != right_neighbor[2]:
                            gap_val = right_neighbor[0] - left_neighbor[0]
                            heapq.heappush(heap, (gap_val, left_neighbor[1], right_neighbor[1]))
                else:
                    pass
                
                current_value[u] = new_val
                new_tuple = (new_val, u, color[u])
                j = bisect.bisect_left(sorted_list, new_tuple)
                sorted_list.insert(j, new_tuple)
                
                left_neighbor = None
                right_neighbor = None
                if j-1 >= 0:
                    left_neighbor = sorted_list[j-1]
                if j+1 < len(sorted_list):
                    right_neighbor = sorted_list[j+1]
                
                if left_neighbor and right_neighbor:
                    if left_neighbor[2] != right_neighbor[2]:
                        key = (left_neighbor[1], right_neighbor[1])
                        invalid.add(key)
                
                if left_neighbor:
                    if left_neighbor[2] != new_tuple[2]:
                        gap_val = new_tuple[0] - left_neighbor[0]
                        heapq.heappush(heap, (gap_val, left_neighbor[1], new_tuple[1]))
                if right_neighbor:
                    if new_tuple[2] != right_neighbor[2]:
                        gap_val = right_neighbor[0] - new_tuple[0]
                        heapq.heappush(heap, (gap_val, new_tuple[1], right_neighbor[1]))
            else:
                u = b; v = c
                if color[u] == color[v]:
                    out_lines.append("0")
                else:
                    found = False
                    while heap:
                        gap_val, node1, node2 = heap[0]
                        key = (node1, node2)
                        if key in invalid:
                            heapq.heappop(heap)
                            if key in invalid:
                                invalid.remove(key)
                            continue
                        else:
                            out_lines.append(str(gap_val))
                            found = True
                            break
                    if not found:
                        out_lines.append("0")
        
        print("\n".join(out_lines))

if __name__ == '__main__':
    main()