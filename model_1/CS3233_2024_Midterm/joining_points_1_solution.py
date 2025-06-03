import sys

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    points = []
    xs = []
    ys = []
    index = 1
    for i in range(n):
        x = int(data[index])
        y = int(data[index+1])
        index += 2
        points.append((x, y))
        xs.append(x)
        ys.append(y)
    
    min_x = min(xs)
    max_x = max(xs)
    min_y = min(ys)
    max_y = max(ys)
    
    s1 = max_x - min_x
    s2 = max_y - min_y
    s0 = max(s1, s2)
    
    candidates = []
    candidates.append((min_x, min_x + s0, min_y, min_y + s0))
    candidates.append((min_x, min_x + s0, max_y - s0, max_y))
    candidates.append((max_x - s0, max_x, min_y, min_y + s0))
    candidates.append((max_x - s0, max_x, max_y - s0, max_y))
    
    if s1 >= s2:
        candidates.append((min_x, max_x, min_y, min_y + s1))
        candidates.append((min_x, max_x, max_y - s1, max_y))
    if s2 >= s1:
        candidates.append((min_x, min_x + s2, min_y, max_y))
        candidates.append((max_x - s2, max_x, min_y, max_y))
    
    for cand in candidates:
        x1, x2, y1, y2 = cand
        valid = True
        for (x, y) in points:
            if x == x1 or x == x2 or y == y1 or y == y2:
                continue
            else:
                valid = False
                break
        if valid:
            print(f"{x1} {x2} {y1} {y2}")
            return
    
    print("Impossible")

if __name__ == "__main__":
    main()