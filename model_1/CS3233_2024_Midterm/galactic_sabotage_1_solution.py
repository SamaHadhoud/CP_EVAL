import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    m = int(data[1])
    A = [0] * (n + 1)
    tokens = list(map(int, data[2:2+n]))
    for i in range(1, n + 1):
        A[i] = tokens[i - 1]
    
    swaps = []
    index = 2 + n
    for i in range(m):
        u = int(data[index])
        v = int(data[index + 1])
        index += 2
        swaps.append((u, v))
    
    P = [0] * (n + 1)
    P[0] = 0
    total_breaks = 0
    for i in range(1, n + 1):
        P[i] = max(P[i - 1], A[i])
        if P[i] == i:
            total_breaks += 1

    out_lines = [str(total_breaks)]
    
    for swap in swaps:
        u, v = swap
        u0 = min(u, v)
        v0 = max(u, v)
        if u0 == v0:
            out_lines.append(str(total_breaks))
            continue
        
        A[u0], A[v0] = A[v0], A[u0]
        
        temp = P[u0 - 1]
        i = u0
        updated = False
        while i <= n:
            candidate = A[i]
            new_val = candidate if candidate > temp else temp
            old_val = P[i]
            
            if i >= v0 and new_val == old_val:
                break
                
            old_break = (old_val == i)
            new_break = (new_val == i)
            if old_break != new_break:
                if old_break:
                    total_breaks -= 1
                else:
                    total_breaks += 1
            
            P[i] = new_val
            temp = new_val
            i += 1

        out_lines.append(str(total_breaks))
    
    print("\n".join(out_lines))

if __name__ == "__main__":
    main()