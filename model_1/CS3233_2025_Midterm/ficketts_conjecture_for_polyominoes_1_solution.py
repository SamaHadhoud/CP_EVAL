import math
from collections import defaultdict

def main():
    import sys
    data = sys.stdin.read().splitlines()
    t = int(data[0].strip())
    index = 1
    out_lines = []
    for _ in range(t):
        R, C = map(int, data[index].split())
        index += 1
        grid = []
        for i in range(R):
            grid.append(data[index].strip())
            index += 1
        
        r0 = 0
        while r0 < R and '#' not in grid[r0]:
            r0 += 1
        if r0 == R:
            out_lines.append("0/1")
            continue
        r1 = R - 1
        while r1 >= 0 and '#' not in grid[r1]:
            r1 -= 1
        col_has_hash = [False] * C
        for j in range(C):
            for i in range(R):
                if grid[i][j] == '#':
                    col_has_hash[j] = True
        c0 = 0
        while c0 < C and not col_has_hash[c0]:
            c0 += 1
        c1 = C - 1
        while c1 >= 0 and not col_has_hash[c1]:
            c1 -= 1
        
        H = r1 - r0 + 1
        W = c1 - c0 + 1
        grid0 = []
        for i in range(r0, r1 + 1):
            grid0.append(grid[i][c0:c1 + 1])
        
        A0 = set()
        border0 = set()
        directions = [(-1, -1), (-1, 0), (-1, 1),
                      (0, -1),           (0, 1),
                      (1, -1),  (1, 0),  (1, 1)]
        for i in range(H):
            for j in range(W):
                if grid0[i][j] == '#':
                    A0.add((i, j))
                    is_border = False
                    for dx, dy in directions:
                        ni, nj = i + dx, j + dy
                        if 0 <= ni < H and 0 <= nj < W:
                            if grid0[ni][nj] != '#':
                                is_border = True
                                break
                        else:
                            is_border = True
                            break
                    if is_border:
                        border0.add((i, j))
        
        rotations = [0, 90, 180, 270]
        best_num = 0
        best_den = 1
        for rot in rotations:
            if rot == 0:
                A_rot = A0
                border_rot = border0
            else:
                A_rot = set()
                border_rot = set()
                if rot == 90:
                    for (i, j) in A0:
                        A_rot.add((j, H - 1 - i))
                    for (i, j) in border0:
                        border_rot.add((j, H - 1 - i))
                elif rot == 180:
                    for (i, j) in A0:
                        A_rot.add((H - 1 - i, W - 1 - j))
                    for (i, j) in border0:
                        border_rot.add((H - 1 - i, W - 1 - j))
                elif rot == 270:
                    for (i, j) in A0:
                        A_rot.add((W - 1 - j, i))
                    for (i, j) in border0:
                        border_rot.add((W - 1 - j, i))
            
            trans_numer = defaultdict(int)
            for a in border0:
                for b in A_rot:
                    dx = a[0] - b[0]
                    dy = a[1] - b[1]
                    trans_numer[(dx, dy)] += 1
            
            trans_denom = defaultdict(int)
            for a in A0:
                for b in border_rot:
                    dx = a[0] - b[0]
                    dy = a[1] - b[1]
                    trans_denom[(dx, dy)] += 1
            
            for key in trans_numer:
                num_val = trans_numer[key]
                den_val = trans_denom.get(key, 0)
                if den_val == 0:
                    continue
                if best_num * den_val < best_den * num_val:
                    best_num = num_val
                    best_den = den_val
        
        if best_num == 0:
            out_lines.append("0/1")
        else:
            g = math.gcd(best_num, best_den)
            best_num //= g
            best_den //= g
            out_lines.append(f"{best_num}/{best_den}")
    
    for line in out_lines:
        print(line)

if __name__ == "__main__":
    main()