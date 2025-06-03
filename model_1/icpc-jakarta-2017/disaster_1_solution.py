import math
from collections import deque

def distance_to_segment(px, py, x1, y1, x2, y2):
    if x1 == x2:
        if y1 > y2:
            y1, y2 = y2, y1
        if py < y1:
            return math.hypot(px - x1, py - y1)
        elif py > y2:
            return math.hypot(px - x1, py - y2)
        else:
            return abs(px - x1)
    else:
        if x1 > x2:
            x1, x2 = x2, x1
        if px < x1:
            return math.hypot(px - x1, py - y1)
        elif px > x2:
            return math.hypot(px - x2, py - y1)
        else:
            return abs(py - y1)

def main():
    data = input().split()
    x1 = int(data[0])
    y1 = int(data[1])
    x2 = int(data[2])
    y2 = int(data[3])
    N = int(data[4])
    circles = []
    for _ in range(N):
        fx, fy, r = map(int, input().split())
        circles.append((fx, fy, r))
    
    relevant_circles = []
    for (fx, fy, r) in circles:
        nx = max(x1, min(x2, fx))
        ny = max(y1, min(y2, fy))
        dx = nx - fx
        dy = ny - fy
        if dx * dx + dy * dy <= r * r:
            relevant_circles.append((fx, fy, r))
    circles = relevant_circles
    n = len(circles)
    
    graph = [[] for _ in range(n + 4)]
    left_boundary = n
    right_boundary = n + 1
    bottom_boundary = n + 2
    top_boundary = n + 3
    
    for i, (fx, fy, r) in enumerate(circles):
        d_left = distance_to_segment(fx, fy, x1, y1, x1, y2)
        if d_left < r:
            graph[i].append(left_boundary)
            graph[left_boundary].append(i)
        
        d_right = distance_to_segment(fx, fy, x2, y1, x2, y2)
        if d_right < r:
            graph[i].append(right_boundary)
            graph[right_boundary].append(i)
        
        d_bottom = distance_to_segment(fx, fy, x1, y1, x2, y1)
        if d_bottom < r:
            graph[i].append(bottom_boundary)
            graph[bottom_boundary].append(i)
        
        d_top = distance_to_segment(fx, fy, x1, y2, x2, y2)
        if d_top < r:
            graph[i].append(top_boundary)
            graph[top_boundary].append(i)
    
    for i in range(n):
        for j in range(i + 1, n):
            fx1, fy1, r1 = circles[i]
            fx2, fy2, r2 = circles[j]
            dx = fx1 - fx2
            dy = fy1 - fy2
            if dx * dx + dy * dy <= (r1 + r2) * (r1 + r2):
                graph[i].append(j)
                graph[j].append(i)
    
    visited = [False] * (n + 4)
    queue = deque([left_boundary])
    visited[left_boundary] = True
    while queue:
        u = queue.popleft()
        if u == right_boundary:
            print("NO")
            return
        for v in graph[u]:
            if not visited[v]:
                visited[v] = True
                queue.append(v)
    
    visited = [False] * (n + 4)
    queue = deque([bottom_boundary])
    visited[bottom_boundary] = True
    while queue:
        u = queue.popleft()
        if u == top_boundary:
            print("NO")
            return
        for v in graph[u]:
            if not visited[v]:
                visited[v] = True
                queue.append(v)
    
    print("YES")

if __name__ == "__main__":
    main()