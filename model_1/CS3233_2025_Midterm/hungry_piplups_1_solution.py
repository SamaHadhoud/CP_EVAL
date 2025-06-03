import sys

MAX_U = 1001000
next_ptr = list(range(MAX_U + 2))
timestamp = [0] * (MAX_U + 2)
now = 0

class FenwTree:
    def __init__(self, size):
        self.n = size
        self.tree = [0] * (self.n + 1)
    
    def update(self, index, delta):
        i = index + 1
        while i <= self.n:
            self.tree[i] += delta
            i += i & -i
            
    def query(self, index):
        if index < 0:
            return 0
        i = index + 1
        s = 0
        while i > 0:
            s += self.tree[i]
            i -= i & -i
        return s
        
    def range_sum(self, l, r):
        if l > r:
            return 0
        return self.query(r) - self.query(l - 1)

def find(x, U_bound):
    if x > U_bound:
        return x
    path = []
    cur = x
    while cur <= U_bound:
        if timestamp[cur] != now:
            break
        if next_ptr[cur] == cur:
            break
        path.append(cur)
        cur = next_ptr[cur]
    if cur > U_bound:
        for node in path:
            next_ptr[node] = cur
        return cur
    if timestamp[cur] != now:
        timestamp[cur] = now
        next_ptr[cur] = cur
    for node in path:
        next_ptr[node] = cur
    return cur

def main():
    global now
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it)); m = int(next(it))
    A = [0] * n
    for i in range(n):
        A[i] = int(next(it))
    
    fenw_trees = [None]
    for x in range(1, 501):
        fenw_trees.append(FenwTree(n))
    
    for i in range(n):
        if 1 <= A[i] <= 500:
            fenw_trees[A[i]].update(i, 1)
    
    out_lines = []
    for _ in range(m):
        t = int(next(it))
        if t == 0:
            p = int(next(it)); x_val = int(next(it))
            idx = p - 1
            old_val = A[idx]
            if 1 <= old_val <= 500:
                fenw_trees[old_val].update(idx, -1)
            if 1 <= x_val <= 500:
                fenw_trees[x_val].update(idx, 1)
            A[idx] = x_val
        else:
            L = int(next(it)); R = int(next(it)); X = int(next(it))
            len_seg = R - L + 1
            U_bound = len_seg + 1000
            now += 1
            freq = [0] * 501
            for x in range(1, 501):
                cnt = fenw_trees[x].range_sum(L - 1, R - 1)
                freq[x] = cnt
            for x in range(1, 501):
                cnt = freq[x]
                for _ in range(cnt):
                    y = find(x, U_bound)
                    if y <= U_bound:
                        next_ptr[y] = find(y + 1, U_bound)
            current = 0
            for j in range(X):
                current = find(current + 1, U_bound)
            out_lines.append(str(current))
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    main()