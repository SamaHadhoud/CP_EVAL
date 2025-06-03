import math
import collections
import sys

epsilon = 1e-6

class Point:
    __slots__ = ('x', 'y')
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __repr__(self):
        return f"({self.x},{self.y})"
    
    def distance_sq(self, other):
        dx = self.x - other.x
        dy = self.y - other.y
        return dx*dx + dy*dy
    
    def distance(self, other):
        return math.sqrt(self.distance_sq(other))
    
    def subtract(self, other):
        return Point(self.x - other.x, self.y - other.y)
    
    def add(self, other):
        return Point(self.x + other.x, self.y + other.y)
    
    def multiply(self, scalar):
        return Point(self.x * scalar, self.y * scalar)
    
    def dot(self, other):
        return self.x * other.x + self.y * other.y
    
    def norm(self):
        return math.sqrt(self.x*self.x + self.y*self.y)
    
    def normalize(self):
        n = self.norm()
        if n < epsilon:
            return Point(0,0)
        return Point(self.x/n, self.y/n)
    
    def rotate(self, angle):
        cos_angle = math.cos(angle)
        sin_angle = math.sin(angle)
        return Point(
            self.x * cos_angle - self.y * sin_angle,
            self.x * sin_angle + self.y * cos_angle
        )

class Circle:
    __slots__ = ('c', 'r')
    def __init__(self, center, radius):
        self.c = center
        self.r = radius
    
    def tangent_points_from_point(self, p):
        d_vec = self.c.subtract(p)
        d_sq = d_vec.x*d_vec.x + d_vec.y*d_vec.y
        if d_sq < self.r*self.r - epsilon:
            return []
        if abs(d_sq - self.r*self.r) < epsilon:
            return [p]
        d = math.sqrt(d_sq)
        angle = math.atan2(d_vec.y, d_vec.x)
        da = math.acos(self.r / d)
        p1 = Point(
            self.c.x + self.r * math.cos(angle - da),
            self.c.y + self.r * math.sin(angle - da)
        )
        p2 = Point(
            self.c.x + self.r * math.cos(angle + da),
            self.c.y + self.r * math.sin(angle + da)
        )
        return [p1, p2]

def common_tangents(c1, c2):
    tangents = []
    d = c1.c.distance(c2.c)
    if d < epsilon:
        return tangents
    v = Point((c2.c.x - c1.c.x)/d, (c2.c.y - c1.c.y)/d)
    for sign in [1, -1]:
        r_diff = sign * c1.r - sign * c2.r
        if abs(r_diff) < epsilon:
            a = v.rotate(math.pi/2)
            offset1 = a.multiply(c1.r)
            tangents.append((c1.c.add(offset1), c2.c.add(offset1)))
            tangents.append((c1.c.add(offset1.multiply(-1)), c2.c.add(offset1.multiply(-1))))
        else:
            ratio = (c1.r - sign * c2.r) / d
            if abs(ratio) > 1:
                continue
            angle = math.acos(ratio)
            a1 = v.rotate(angle)
            a2 = v.rotate(-angle)
            offset1 = a1.multiply(c1.r)
            offset2 = a2.multiply(c1.r)
            point1_1 = c1.c.add(offset1)
            point1_2 = c1.c.add(offset2)
            point2_1 = c2.c.add(v.multiply(sign * c1.r)).add(offset1.rotate(math.pi))
            point2_2 = c2.c.add(v.multiply(sign * c1.r)).add(offset2.rotate(math.pi))
            tangents.append((point1_1, point2_1))
            tangents.append((point1_2, point2_2))
    for sign in [1, -1]:
        r_sum = c1.r + sign * c2.r
        ratio = r_sum / d
        if ratio > 1:
            continue
        angle = math.acos(ratio)
        a1 = v.rotate(angle)
        a2 = v.rotate(-angle)
        offset1 = a1.multiply(c1.r)
        offset2 = a2.multiply(c1.r)
        point1_1 = c1.c.add(offset1)
        point1_2 = c1.c.add(offset2)
        point2_1 = c2.c.add(offset1.rotate(math.pi)).add(v.multiply(-sign * c2.r))
        point2_2 = c2.c.add(offset2.rotate(math.pi)).add(v.multiply(-sign * c2.r))
        tangents.append((point1_1, point2_1))
        tangents.append((point1_2, point2_2))
    return tangents

def is_segment_safe_from_circle(a, b, circle):
    vx = b.x - a.x
    vy = b.y - a.y
    len_sq = vx*vx + vy*vy
    if len_sq < epsilon:
        d_sq = (a.x - circle.c.x)**2 + (a.y - circle.c.y)**2
        return d_sq >= (circle.r - epsilon)**2
    dx = circle.c.x - a.x
    dy = circle.c.y - a.y
    dot = dx * vx + dy * vy
    t_val = dot / len_sq
    if t_val < 0:
        t_val = 0
    elif t_val > 1:
        t_val = 1
    projx = a.x + t_val * vx
    projy = a.y + t_val * vy
    d_sq = (circle.c.x - projx)**2 + (circle.c.y - projy)**2
    if d_sq < (circle.r - epsilon)**2:
        return False
    return True

def segment_safe(a, b, circles, xL, yL, xR, yR):
    for circle in circles:
        if not is_segment_safe_from_circle(a, b, circle):
            return False
    return True

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    it = iter(data)
    n = int(next(it)); xL = int(next(it)); yL = int(next(it)); xR = int(next(it)); yR = int(next(it))
    xs = int(next(it)); ys = int(next(it))
    xt = int(next(it)); yt = int(next(it))
    circles = []
    for i in range(n):
        x = int(next(it)); y = int(next(it)); r = int(next(it))
        circles.append(Circle(Point(x,y), r))
    
    start = Point(xs, ys)
    target = Point(xt, yt)
    
    if segment_safe(start, target, circles, xL, yL, xR, yR):
        print(0)
        return
    
    points = []
    points.append(start)
    points.append(target)
    corners = [Point(xL,yL), Point(xL,yR), Point(xR,yL), Point(xR,yR)]
    points.extend(corners)
    
    for circle in circles:
        tangents_start = circle.tangent_points_from_point(start)
        for t in tangents_start:
            if xL - epsilon <= t.x <= xR + epsilon and yL - epsilon <= t.y <= yR + epsilon:
                points.append(t)
        tangents_target = circle.tangent_points_from_point(target)
        for t in tangents_target:
            if xL - epsilon <= t.x <= xR + epsilon and yL - epsilon <= t.y <= yR + epsilon:
                points.append(t)
    
    for i in range(len(circles)):
        for j in range(i+1, len(circles)):
            tangents = common_tangents(circles[i], circles[j])
            for tangent_seg in tangents:
                p1, p2 = tangent_seg
                if xL - epsilon <= p1.x <= xR + epsilon and yL - epsilon <= p1.y <= yR + epsilon:
                    points.append(p1)
                if xL - epsilon <= p2.x <= xR + epsilon and yL - epsilon <= p2.y <= yR + epsilon:
                    points.append(p2)
    
    unique_points = []
    for p in points:
        is_duplicate = False
        for q in unique_points:
            if abs(p.x - q.x) <= epsilon and abs(p.y - q.y) <= epsilon:
                is_duplicate = True
                break
        if not is_duplicate:
            unique_points.append(p)
    points = unique_points
    n_points = len(points)
    
    graph = [[] for _ in range(n_points)]
    for i in range(n_points):
        for j in range(i+1, n_points):
            if segment_safe(points[i], points[j], circles, xL, yL, xR, yR):
                graph[i].append(j)
                graph[j].append(i)
    
    parent = [-1] * n_points
    queue = collections.deque()
    queue.append(0)
    parent[0] = 0
    found = False
    while queue:
        node = queue.popleft()
        if node == 1:
            found = True
            break
        for neighbor in graph[node]:
            if parent[neighbor] == -1:
                parent[neighbor] = node
                queue.append(neighbor)
    
    if not found:
        print("No path found")
        return
    
    path = []
    node = 1
    while node != 0:
        path.append(node)
        node = parent[node]
    path.append(0)
    path.reverse()
    
    intermediates = path[1:-1]
    print(len(intermediates))
    for idx in intermediates:
        p = points[idx]
        print(f"{p.x} {p.y}")

if __name__ == "__main__":
    main()