import sys

class Fenw:
    def __init__(self, n):
        self.n = n
        self.tree = [0] * (n + 1)
    
    def update(self, index, delta):
        i = index
        while i <= self.n:
            self.tree[i] += delta
            i += i & -i
    
    def query(self, index):
        s = 0
        i = index
        while i:
            s += self.tree[i]
            i -= i & -i
        return s

    def range_query(self, l, r):
        if l > r:
            return 0
        return self.query(r) - self.query(l - 1)

class SegmentTree:
    def __init__(self, data):
        self.n = len(data)
        self.size = 1
        while self.size < self.n:
            self.size *= 2
        self.tree = [-10**18] * (2 * self.size)
        for i in range(self.n):
            self.tree[self.size + i] = data[i]
        for i in range(self.size - 1, 0, -1):
            self.tree[i] = max(self.tree[2*i], self.tree[2*i+1])
    
    def update(self, index, value):
        i = self.size + index
        self.tree[i] = value
        i //= 2
        while i:
            self.tree[i] = max(self.tree[2*i], self.tree[2*i+1])
            i //= 2

    def query(self, l, r):
        l0 = l
        r0 = r
        l += self.size
        r += self.size
        res = -10**18
        while l <= r:
            if l % 2 == 1:
                res = max(res, self.tree[l])
                l += 1
            if r % 2 == 0:
                res = max(res, self.tree[r])
                r -= 1
            l //= 2
            r //= 2
        return res

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    k = int(data[1])
    P = list(map(int, data[2:2 + n]))
    constraints = []
    index = 2 + n
    for i in range(k):
        a = int(data[index])
        b = int(data[index + 1])
        c = int(data[index + 2])
        index += 3
        constraints.append((a, b, c))
    
    ans = [0] * (n + 1)
    base_prefix = [0] * (n + 1)
    for i in range(1, n + 1):
        if P[i - 1] != 0:
            ans[i] = P[i - 1]
        else:
            ans[i] = -1
        base_prefix[i] = base_prefix[i - 1] + ans[i]
    
    available_tree = Fenw(n)
    flip_tree = Fenw(n)
    
    base_arr = [-10**18] * n
    for i in range(1, n + 1):
        if P[i - 1] == 0:
            available_tree.update(i, 1)
            base_arr[i - 1] = i
        else:
            base_arr[i - 1] = -10**18
    
    seg_tree = SegmentTree(base_arr)
    
    constraints_by_B = {}
    for B in range(1, n + 1):
        constraints_by_B[B] = []
    for (a, b, c) in constraints:
        constraints_by_B[b].append((a, c))
    
    for B in range(1, n + 1):
        cons_list = constraints_by_B[B]
        cons_list.sort(key=lambda x: x[0], reverse=True)
        
        for (A, C) in cons_list:
            total_base = base_prefix[B] - base_prefix[A - 1]
            already_flips = flip_tree.range_query(A, B)
            current_sum = total_base + 2 * already_flips
            d = C - current_sum
            if d <= 0:
                continue
            t = (d + 1) // 2
            count_avail = available_tree.range_query(A, B)
            if count_avail < t:
                print("Impossible")
                return
            for _ in range(t):
                max_pos_val = seg_tree.query(A - 1, B - 1)
                if max_pos_val < A:
                    print("Impossible")
                    return
                pos = max_pos_val
                seg_tree.update(pos - 1, -10**18)
                available_tree.update(pos, -1)
                flip_tree.update(pos, 1)
                ans[pos] = 1
    
    res = []
    for i in range(1, n + 1):
        res.append(str(ans[i]))
    print(" ".join(res))

if __name__ == "__main__":
    main()