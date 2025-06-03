import sys

def main():
    data = sys.stdin.read().split()
    t = int(data[0])
    index = 1
    output_lines = []
    for _ in range(t):
        n = int(data[index]); index += 1
        X = []
        for i in range(n):
            row = list(map(int, data[index:index+n]))
            index += n
            X.append(row)
        
        bad_rows = []
        for i in range(n):
            if len(set(X[i])) == n:
                bad_rows.append(i)
                
        bad_cols = []
        for j in range(n):
            col = [X[i][j] for i in range(n)]
            if len(set(col)) == n:
                bad_cols.append(j)
                
        r = len(bad_rows)
        c = len(bad_cols)
        k = min(r, c)
        modifications = []
        
        for i in range(k):
            i_row = bad_rows[i]
            j_col = bad_cols[i]
            a = X[i_row][j_col]
            if a != 1:
                v = 1
            else:
                v = 2
            modifications.append((i_row, j_col, v))
            
        for i in range(k, r):
            i_row = bad_rows[i]
            found = False
            for j in range(n-1, -1, -1):
                a0 = X[i_row][j]
                list_A = []
                for jj in range(n):
                    if jj == j: 
                        continue
                    list_A.append(X[i_row][jj])
                list_B = []
                for ii in range(n):
                    if ii == i_row:
                        continue
                    list_B.append(X[ii][j])
                has_dup_A = (len(list_A) != len(set(list_A)))
                has_dup_B = (len(list_B) != len(set(list_B)))
                if has_dup_A and has_dup_B:
                    if a0 != 1:
                        v = 1
                    else:
                        v = 2
                    modifications.append((i_row, j, v))
                    found = True
                    break
                elif has_dup_A and (not has_dup_B):
                    set_B = set(list_B)
                    v = min(set_B)
                    modifications.append((i_row, j, v))
                    found = True
                    break
                elif (not has_dup_A) and has_dup_B:
                    set_A = set(list_A)
                    v = min(set_A)
                    modifications.append((i_row, j, v))
                    found = True
                    break
                else:
                    set_A = set(list_A)
                    set_B = set(list_B)
                    common = set_A & set_B
                    if common:
                        v = min(common)
                        modifications.append((i_row, j, v))
                        found = True
                        break
            if not found:
                j = 0
                a0 = X[i_row][0]
                list_A = X[i_row][1:]
                set_A = set(list_A)
                if set_A:
                    v = next(iter(set_A))
                else:
                    v = 1
                modifications.append((i_row, j, v))
        
        for j in range(k, c):
            j_col = bad_cols[j]
            found = False
            for i in range(n-1, -1, -1):
                a0 = X[i][j_col]
                list_A = []
                for jj in range(n):
                    if jj == j_col:
                        continue
                    list_A.append(X[i][jj])
                list_B = []
                for ii in range(n):
                    if ii == i:
                        continue
                    list_B.append(X[ii][j_col])
                has_dup_A = (len(list_A) != len(set(list_A)))
                has_dup_B = (len(list_B) != len(set(list_B)))
                if has_dup_A and has_dup_B:
                    if a0 != 1:
                        v = 1
                    else:
                        v = 2
                    modifications.append((i, j_col, v))
                    found = True
                    break
                elif has_dup_A and (not has_dup_B):
                    set_B = set(list_B)
                    v = min(set_B)
                    modifications.append((i, j_col, v))
                    found = True
                    break
                elif (not has_dup_A) and has_dup_B:
                    set_A = set(list_A)
                    v = min(set_A)
                    modifications.append((i, j_col, v))
                    found = True
                    break
                else:
                    set_A = set(list_A)
                    set_B = set(list_B)
                    common = set_A & set_B
                    if common:
                        v = min(common)
                        modifications.append((i, j_col, v))
                        found = True
                        break
            if not found:
                i = 0
                a0 = X[0][j_col]
                list_A = X[0][:j_col] + X[0][j_col+1:]
                set_A = set(list_A)
                if set_A:
                    v = next(iter(set_A))
                else:
                    v = 1
                modifications.append((i, j_col, v))
        
        output_lines.append(str(len(modifications)))
        for (i, j, v) in modifications:
            output_lines.append(f"{i+1} {j+1} {v}")
            
    print("\n".join(output_lines))

if __name__ == '__main__':
    main()