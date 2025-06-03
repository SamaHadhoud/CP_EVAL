import sys

def main():
    data = sys.stdin.read().splitlines()
    if not data: 
        return
    
    R, C, Q = map(int, data[0].split())
    grid = []
    for i in range(1, 1+R):
        grid.append(data[i].strip())
    
    base = 131
    max_len = max(R, C)
    pow_base = [1] * (max_len + 1)
    for i in range(1, max_len + 1):
        pow_base[i] = pow_base[i-1] * base

    H_row = [[0] * (C + 1) for _ in range(R)]
    for r in range(R):
        for c in range(C):
            char_val = ord(grid[r][c]) - ord('A') + 1
            H_row[r][c+1] = H_row[r][c] * base + char_val

    H_col = [[0] * (R + 1) for _ in range(C)]
    for c in range(C):
        for r in range(R):
            char_val = ord(grid[r][c]) - ord('A') + 1
            H_col[c][r+1] = H_col[c][r] * base + char_val

    out_lines = []
    index = 1 + R
    for _ in range(Q):
        S = data[index].strip()
        index += 1
        L = len(S)
        if L > R + C - 1:
            out_lines.append("0")
            continue

        H_S = [0] * (L + 1)
        for j in range(L):
            char_val = ord(S[j]) - ord('A') + 1
            H_S[j+1] = H_S[j] * base + char_val

        low_i = max(0, L - 1 - (R - 1))
        high_i = min(L - 1, C - 1)
        ans = 0
        for i in range(low_i, high_i + 1):
            h_len = i + 1
            v_len = L - i - 1
            if v_len < 0:
                continue
            if v_len > 0:
                max_r = R - 1 - v_len
            else:
                max_r = R - 1
            for r in range(0, max_r + 1):
                for d in range(i, C):
                    start_col = d - i
                    if start_col < 0:
                        continue
                    hash_row = H_row[r][d+1] - H_row[r][start_col] * pow_base[h_len]
                    if hash_row != H_S[h_len]:
                        continue
                    if v_len > 0:
                        hash_col = H_col[d][r+1 + v_len] - H_col[d][r+1] * pow_base[v_len]
                        expected_vert = H_S[h_len + v_len] - H_S[h_len] * pow_base[v_len]
                        if hash_col != expected_vert:
                            continue
                    ans += 1
        out_lines.append(str(ans))
    
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()