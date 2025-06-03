import sys

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it))
    q = int(next(it))
    R = [int(next(it)) for _ in range(n)]
    C = [int(next(it)) for _ in range(n)]
    
    r_parity = [x % 2 for x in R]
    c_parity = [x % 2 for x in C]
    
    row0_group = [-1] * n
    col0_group = [-1] * n
    row1_group = [-1] * n
    col1_group = [-1] * n
    
    current_start = -1
    for i in range(n):
        if r_parity[i] == 0:
            if i == 0 or r_parity[i-1] != 0:
                current_start = i
            row0_group[i] = current_start
    
    current_start = -1
    for j in range(n):
        if c_parity[j] == 0:
            if j == 0 or c_parity[j-1] != 0:
                current_start = j
            col0_group[j] = current_start
    
    current_start = -1
    for i in range(n):
        if r_parity[i] == 1:
            if i == 0 or r_parity[i-1] != 1:
                current_start = i
            row1_group[i] = current_start
    
    current_start = -1
    for j in range(n):
        if c_parity[j] == 1:
            if j == 0 or c_parity[j-1] != 1:
                current_start = j
            col1_group[j] = current_start
    
    out_lines = []
    for _ in range(q):
        ra = int(next(it)) - 1
        ca = int(next(it)) - 1
        rb = int(next(it)) - 1
        cb = int(next(it)) - 1
        
        if r_parity[ra] != r_parity[rb]:
            out_lines.append("NO")
            continue
            
        if r_parity[ra] == 0:
            if row0_group[ra] == row0_group[rb] and col0_group[ca] == col0_group[cb]:
                out_lines.append("YES")
            else:
                out_lines.append("NO")
        else:
            if row1_group[ra] == row1_group[rb] and col1_group[ca] == col1_group[cb]:
                out_lines.append("YES")
            else:
                out_lines.append("NO")
                
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()