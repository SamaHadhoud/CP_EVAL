import sys

def main():
    data = sys.stdin.read().splitlines()
    if not data:
        return
    first_line = data[0].split()
    if not first_line:
        return
    N = int(first_line[0])
    K = int(first_line[1])
    s = data[1].strip()
    M = len(s)
    
    min_size = [0] * M
    if M > 0:
        if s[-1] == '=':
            min_size[M-1] = 1
        else:
            min_size[M-1] = 2
        for i in range(M-2, -1, -1):
            min_size[i] = min_size[i+1] + 1
    
    L = 1
    R = N
    guesses = []
    
    for i in range(M):
        if R < L:
            print(-1)
            return
            
        current_size = R - L + 1
        if current_size < min_size[i]:
            print(-1)
            return
            
        c = s[i]
        if c == '=':
            if i != M-1:
                print(-1)
                return
            else:
                if L == R:
                    guesses.append(L)
                else:
                    print(-1)
                    return
        else:
            if i == M-1:
                next_min = 1
            else:
                next_min = min_size[i+1]
                
            if c == '<':
                cand1 = (L + R + 1) // 2
                cand2 = L + next_min
                g = cand1 if cand1 > cand2 else cand2
                if g > R:
                    print(-1)
                    return
                guesses.append(g)
                R = g - 1
            else:
                cand1 = (L + R - 1) // 2
                cand2 = R - next_min
                g = cand1 if cand1 < cand2 else cand2
                if g < L:
                    print(-1)
                    return
                guesses.append(g)
                L = g + 1
                
    print(" ".join(str(x) for x in guesses))

if __name__ == '__main__':
    main()