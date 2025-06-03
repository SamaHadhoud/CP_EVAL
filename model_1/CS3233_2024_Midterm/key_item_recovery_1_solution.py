n = int(input().strip())
matrix = []
for _ in range(n):
    matrix.append(list(map(int, input().split())))
edges = []
for i in range(2, n + 1):
    row = matrix[i - 1]
    M = 0
    for j in range(1, i):
        if row[j - 1] > M:
            M = row[j - 1]
    u = min(i, M)
    v = max(i, M)
    edges.append((u, v))
edges.sort(key=lambda x: (-x[0], x[1]))
for u, v in edges:
    print(f"{u} {v}")