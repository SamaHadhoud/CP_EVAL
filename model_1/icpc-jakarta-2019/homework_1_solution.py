n = int(input().strip())
A = list(map(int, input().split()))
if n == 1:
    print(A[0])
else:
    B = A[1:] + [A[0]]
    print(' '.join(map(str, B)))