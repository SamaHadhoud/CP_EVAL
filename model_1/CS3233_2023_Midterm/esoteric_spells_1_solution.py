import sys

def main():
    data = sys.stdin.read().split()
    c = int(data[0])
    index = 1
    out_lines = []
    for _ in range(c):
        n = int(data[index]); index += 1
        a = list(map(int, data[index:index+n]))
        index += n
        
        T = 0
        for num in a:
            T |= num
            
        x = [0] * n
        for j in range(61):
            bit = 1 << j
            if T & bit:
                L = []
                for i in range(n):
                    if a[i] & bit:
                        L.append(i)
                if len(L) % 2 == 0:
                    if L:
                        L.pop()
                for idx in L:
                    x[idx] |= bit
                    
        out_lines.append(str(T))
        out_lines.append(" ".join(map(str, x)))
    
    print("\n".join(out_lines))

if __name__ == "__main__":
    main()