import sys
from collections import defaultdict

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    m_val = int(data[1])
    edges = []
    index = 2
    for i in range(n):
        u = int(data[index])
        v = int(data[index + 1])
        w = int(data[index + 2])
        index += 3
        edges.append((u, v, w))
    
    edges.sort(key=lambda x: x[2])
    
    trees = 0
    slots = defaultdict(int)
    
    for u, v, w in edges:
        if slots[u] > 0:
            slots[u] -= 1
            slots[v] += m_val
        else:
            trees += 1
            slots[u] += m_val - 1
            slots[v] += m_val
    
    print(trees)

if __name__ == "__main__":
    main()