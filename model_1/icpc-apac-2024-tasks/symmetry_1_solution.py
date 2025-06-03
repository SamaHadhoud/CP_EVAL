import math

def convex_hull(points, tol=1e-8):
    points = list(points)
    if len(points) <= 1:
        return points
    points = sorted(points, key=lambda x: (x[0], x[1]))
    def cross(o, a, b):
        return (a[0]-o[0])*(b[1]-o[1]) - (a[1]-o[1])*(b[0]-o[0])
    lower = []
    for p in points:
        while len(lower) >= 2 and cross(lower[-2], lower[-1], p) <= tol:
            lower.pop()
        lower.append(p)
    upper = []
    for p in reversed(points):
        while len(upper) >= 2 and cross(upper[-2], upper[-1], p) <= tol:
            upper.pop()
        upper.append(p)
    return lower[:-1] + upper[:-1]

def is_point_on_segment(a, b, p, tol=1e-8):
    cross_product = (b[0]-a[0])*(p[1]-a[1]) - (b[1]-a[1])*(p[0]-a[0])
    if abs(cross_product) > tol:
        return False
    if min(a[0], b[0]) - tol <= p[0] <= max(a[0], b[0]) + tol and min(a[1], b[1]) - tol <= p[1] <= max(a[1], b[1]) + tol:
        return True
    return False

def main():
    import sys
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    vertices = []
    index = 1
    for i in range(n):
        x = float(data[index]); y = float(data[index+1]); index += 2
        vertices.append((x, y))
    if n % 2 == 0:
        c0_x = (vertices[0][0] + vertices[n//2][0]) / 2.0
        c0_y = (vertices[0][1] + vertices[n//2][1]) / 2.0
        symmetric_ok = True
        for i in range(n):
            j = (i + n//2) % n
            sym_x = 2*c0_x - vertices[i][0]
            sym_y = 2*c0_y - vertices[i][1]
            if abs(sym_x - vertices[j][0]) > 1e-8 or abs(sym_y - vertices[j][1]) > 1e-8:
                symmetric_ok = False
                break
        if symmetric_ok:
            area = 0.0
            for i in range(n):
                x1, y1 = vertices[i]
                x2, y2 = vertices[(i+1) % n]
                area += x1 * y2 - x2 * y1
            area = abs(area) / 2.0
            print("{:.15f}".format(area))
            return

    candidate_centers = set()
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            cx = (vertices[i][0] + vertices[j][0]) / 2.0
            cy = (vertices[i][1] + vertices[j][1]) / 2.0
            key = (round(cx, 10), round(cy, 10))
            candidate_centers.add(key)

    equations_set = set()
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            for k in range(n):
                xi, yi = vertices[i]
                xj, yj = vertices[j]
                xk, yk = vertices[k]
                A = -2 * (yj - yi)
                B = 2 * (xj - xi)
                C = (xj - xi) * (yi + yk) - (yj - yi) * (xi + xk)
                if A == 0 and B == 0:
                    if C == 0:
                        continue
                    else:
                        continue
                g = math.gcd(math.gcd(abs(A), abs(B)), abs(C)) if C != 0 else math.gcd(abs(A), abs(B))
                if g == 0:
                    continue
                A //= g; B //= g; C //= g
                if A < 0:
                    A, B, C = -A, -B, -C
                elif A == 0 and B < 0:
                    B, C = -B, -C
                equations_set.add((A, B, C))
    for i in range(n):
        for j in range(n):
            for k in range(n):
                if j == k:
                    continue
                xi, yi = vertices[i]
                xj, yj = vertices[j]
                xk, yk = vertices[k]
                A = 2 * (yj - yk)
                B = 2 * (xk - xj)
                C = (yj - yk) * xi + (xk - xj) * yi + (yj * xk - xj * yk)
                if A == 0 and B == 0:
                    if C == 0:
                        continue
                    else:
                        continue
                g = math.gcd(math.gcd(abs(A), abs(B)), abs(C)) if C != 0 else math.gcd(abs(A), abs(B))
                if g == 0:
                    continue
                A //= g; B //= g; C //= g
                if A < 0:
                    A, B, C = -A, -B, -C
                elif A == 0 and B < 0:
                    B, C = -B, -C
                equations_set.add((A, B, C))
    equations_list = list(equations_set)
    if len(equations_list) <= 1000:
        num_eq = len(equations_list)
        for i in range(num_eq):
            A1, B1, C1 = equations_list[i]
            for j in range(i+1, num_eq):
                A2, B2, C2 = equations_list[j]
                det = A1 * B2 - A2 * B1
                if abs(det) < 1e-8:
                    continue
                cx = (C1 * B2 - C2 * B1) / det
                cy = (A1 * C2 - A2 * C1) / det
                key = (round(cx, 10), round(cy, 10))
                candidate_centers.add(key)
    min_area = None
    for center in candidate_centers:
        cx, cy = center
        S = []
        for v in vertices:
            S.append(v)
            sym_pt = (2*cx - v[0], 2*cy - v[1])
            S.append(sym_pt)
        hull = convex_hull(S)
        valid = True
        for v in vertices:
            found = False
            for p in hull:
                if abs(p[0]-v[0]) < 1e-8 and abs(p[1]-v[1]) < 1e-8:
                    found = True
                    break
            if found:
                continue
            for idx in range(len(hull)):
                a = hull[idx]
                b = hull[(idx+1) % len(hull)]
                if is_point_on_segment(a, b, v, 1e-8):
                    found = True
                    break
            if not found:
                valid = False
                break
        if not valid:
            continue
        area = 0.0
        m = len(hull)
        for idx in range(m):
            x1, y1 = hull[idx]
            x2, y2 = hull[(idx+1) % m]
            area += x1 * y2 - x2 * y1
        area = abs(area) / 2.0
        if min_area is None or area < min_area:
            min_area = area
    if min_area is None:
        print(-1)
    else:
        print("{:.15f}".format(min_area))

if __name__ == '__main__':
    main()