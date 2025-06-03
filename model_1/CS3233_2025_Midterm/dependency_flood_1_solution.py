import sys
from collections import deque

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    m = int(next(it))
    K_val = int(next(it))
    
    edges_set = set()
    adj_out = [[] for _ in range(n+1)]
    adj_in = [[] for _ in range(n+1)]
    
    for _ in range(m):
        u = int(next(it))
        v = int(next(it))
        edges_set.add((u, v))
        adj_out[u].append(v)
        adj_in[v].append(u)
    
    dp_in = [1] * (n+1)
    dp_out = [1] * (n+1)
    
    for i in range(1, n+1):
        for j in adj_in[i]:
            if dp_in[j] + 1 > dp_in[i]:
                dp_in[i] = dp_in[j] + 1
                
    for i in range(n, 0, -1):
        for j in adj_out[i]:
            if dp_out[j] + 1 > dp_out[i]:
                dp_out[i] = dp_out[j] + 1
                
    q = int(next(it))
    output_lines = []
    
    for _ in range(q):
        c = int(next(it))
        d = int(next(it))
        if (c, d) in edges_set:
            output_lines.append("accept")
        else:
            if dp_in[c] + dp_out[d] >= K_val + 1:
                output_lines.append("reject")
            else:
                output_lines.append("accept")
                edges_set.add((c, d))
                adj_out[c].append(d)
                adj_in[d].append(c)
                
                new_in = dp_in[c] + 1
                if new_in > dp_in[d] and new_in <= K_val:
                    dp_in[d] = new_in
                    queue_in = deque([d])
                    while queue_in:
                        u = queue_in.popleft()
                        for w in adj_out[u]:
                            candidate = dp_in[u] + 1
                            if candidate > dp_in[w] and candidate <= K_val:
                                dp_in[w] = candidate
                                queue_in.append(w)
                                
                new_out = dp_out[d] + 1
                if new_out > dp_out[c] and new_out <= K_val:
                    dp_out[c] = new_out
                    queue_out = deque([c])
                    while queue_out:
                        u = queue_out.popleft()
                        for w in adj_in[u]:
                            candidate = dp_out[u] + 1
                            if candidate > dp_out[w] and candidate <= K_val:
                                dp_out[w] = candidate
                                queue_out.append(w)
                                
    print("\n".join(output_lines))

if __name__ == "__main__":
    main()