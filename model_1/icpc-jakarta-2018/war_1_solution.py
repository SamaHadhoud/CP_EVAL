import sys

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    N = int(next(it))
    M = int(next(it))
    Q = int(next(it))
    
    artilleries = []
    for _ in range(N):
        x = int(next(it))
        y = int(next(it))
        artilleries.append((x, y))
    
    walls = []
    for _ in range(M):
        x1 = int(next(it))
        x2 = int(next(it))
        y0 = int(next(it))
        walls.append((x1, x2, y0))
    
    groups_by_key = {}
    for i in range(N):
        x_a, y_a = artilleries[i]
        bitmask = 0
        for j in range(M):
            x1, x2, y0_wall = walls[j]
            if y_a > y0_wall:
                bitmask |= (1 << j)
        if bitmask not in groups_by_key:
            groups_by_key[bitmask] = []
        groups_by_key[bitmask].append(i)
    
    out_lines = []
    for _ in range(Q):
        x_t = int(next(it))
        y_t = int(next(it))
        total_visible = 0
        
        for bitmask, art_list in groups_by_key.items():
            walls_in_group = []
            for j in range(M):
                if bitmask & (1 << j):
                    walls_in_group.append(j)
            if not walls_in_group:
                total_visible += len(art_list)
                continue
                
            for art_idx in art_list:
                x_a, y_a = artilleries[art_idx]
                blocked = False
                for j in walls_in_group:
                    x1, x2, y0_wall = walls[j]
                    base = y_a - y_t
                    L = x_t * (y_a - y0_wall) + x_a * (y0_wall - y_t)
                    left_bound = x1 * base
                    right_bound = x2 * base
                    if left_bound <= L <= right_bound:
                        blocked = True
                        break
                if not blocked:
                    total_visible += 1
        out_lines.append(str(total_visible))
    
    sys.stdout.write("\n".join(out_lines) + "\n")

if __name__ == "__main__":
    main()