import sys

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    edges1 = []
    index = 1
    for _ in range(n - 1):
        u = int(data[index])
        v = int(data[index + 1])
        index += 2
        if u > v:
            u, v = v, u
        edges1.append((u, v))
    
    edges2_set = set()
    for _ in range(n - 1):
        u = int(data[index])
        v = int(data[index + 1])
        index += 2
        if u > v:
            u, v = v, u
        edges2_set.add((u, v))
    
    count = 0
    for edge in edges1:
        if edge not in edges2_set:
            count += 1
            
    print(count)

if __name__ == "__main__":
    main()