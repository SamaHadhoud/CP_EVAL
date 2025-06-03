import sys

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    A = []
    index = 1
    for i in range(n):
        row = list(map(int, data[index:index+n]))
        index += n
        A.append(row)
    
    for i in range(n):
        for j in range(i + 1, n):
            if (A[i][j] + A[j][i]) % 2 != 0:
                print(-1)
                return
                
    is_abs_sym = True
    for i in range(n):
        for j in range(n):
            if abs(A[i][j]) != abs(A[j][i]):
                is_abs_sym = False
                break
        if not is_abs_sym:
            break
            
    if is_abs_sym:
        print(1)
        for i in range(n):
            print(" ".join(map(str, A[i])))
    else:
        print(2)
        X1 = []
        for i in range(n):
            row = []
            for j in range(n):
                if i == j:
                    row.append(0)
                else:
                    row.append((A[i][j] - A[j][i]) // 2)
            print(" ".join(map(str, row)))
        
        for i in range(n):
            row = []
            for j in range(n):
                if i == j:
                    row.append(A[i][i])
                else:
                    row.append((A[i][j] + A[j][i]) // 2)
            print(" ".join(map(str, row)))

if __name__ == "__main__":
    main()