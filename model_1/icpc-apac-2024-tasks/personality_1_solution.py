import sys

def main():
    data = sys.stdin.read().splitlines()
    if not data:
        print(-1)
        return
    n, m, k_val = map(int, data[0].split())
    grid = []
    for i in range(1, 1 + n):
        grid.append(data[i].strip())
    
    non_dot_positions = []
    valid = []
    for i in range(n):
        positions = []
        for j in range(m):
            if grid[i][j] != '.':
                positions.append(j)
        non_dot_positions.append(positions)
        valid.append(len(positions) >= k_val)
    
    if k_val == 0:
        print(f"1 {n}")
        return

    for b in range(1, n):
        if not valid[b]:
            continue
        for a in range(b - 1, -1, -1):
            if not valid[a]:
                continue
            list_a = non_dot_positions[a]
            list_b = non_dot_positions[b]
            da = len(list_a)
            db = len(list_b)
            count = 0
            if da <= db:
                L = da
                for idx, j in enumerate(list_a):
                    if grid[b][j] != '.' and grid[a][j] == grid[b][j]:
                        count += 1
                        if count >= k_val:
                            break
                    remaining = L - idx - 1
                    if count + remaining < k_val:
                        break
            else:
                L = db
                for idx, j in enumerate(list_b):
                    if grid[a][j] != '.' and grid[a][j] == grid[b][j]:
                        count += 1
                        if count >= k_val:
                            break
                    remaining = L - idx - 1
                    if count + remaining < k_val:
                        break
            if count >= k_val:
                print(f"{a + 1} {b + 1}")
                return
    print(-1)

if __name__ == "__main__":
    main()