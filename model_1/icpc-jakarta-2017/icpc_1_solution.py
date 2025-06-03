def main():
    import sys
    input = sys.stdin.read().split()
    idx = 0
    N = int(input[idx])
    idx += 1
    M = int(input[idx])
    idx += 1
    T = list(map(int, input[idx:idx+M]))
    idx += M
    
    max_solved = -1
    best_team = -1
    
    for i in range(N):
        S = list(map(int, input[idx:idx+M]))
        idx += M
        count = 0
        for j in range(M):
            if S[j] == T[j]:
                count += 1
        if count > max_solved:
            max_solved = count
            best_team = i
    
    print(best_team + 1)

if __name__ == "__main__":
    main()