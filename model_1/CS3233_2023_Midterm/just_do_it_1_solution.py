MOD = 1000003233

import sys
sys.setrecursionlimit(1 << 25)

class SegmentTree:
    def __init__(self, n):
        self.n = n
        self.size = 1
        while self.size < n:
            self.size *= 2
        self.t_min = [0] * (2 * self.size)
        self.t_max = [0] * (2 * self.size)
        self.t_prod = [0] * (2 * self.size)
        self.lazy_min_set = [False] * (2 * self.size)
        self.lazy_min_val = [0] * (2 * self.size)
        self.lazy_max_set = [False] * (2 * self.size)
        self.lazy_max_val = [0] * (2 * self.size)
        
    def apply_min(self, node, l, r, x):
        count = r - l + 1
        self.t_min[node] = (x * count) % MOD
        self.t_prod[node] = (x * self.t_max[node]) % MOD
        self.lazy_min_set[node] = True
        self.lazy_min_val[node] = x
        
    def apply_max(self, node, l, r, x):
        count = r - l + 1
        self.t_max[node] = (x * count) % MOD
        self.t_prod[node] = (self.t_min[node] * x) % MOD
        self.lazy_max_set[node] = True
        self.lazy_max_val[node] = x
        
    def push(self, node, l, r):
        if l == r:
            return
        mid = (l + r) // 2
        left_node = 2 * node + 1
        right_node = 2 * node + 2
        if self.lazy_min_set[node]:
            self.apply_min(left_node, l, mid, self.lazy_min_val[node])
            self.apply_min(right_node, mid+1, r, self.lazy_min_val[node])
            self.lazy_min_set[node] = False
        if self.lazy_max_set[node]:
            self.apply_max(left_node, l, mid, self.lazy_max_val[node])
            self.apply_max(right_node, mid+1, r, self.lazy_max_val[node])
            self.lazy_max_set[node] = False

    def update_min_range(self, l, r, x, node=0, segl=0, segr=None):
        if segr is None:
            segr = self.size - 1
        if r < segl or segr < l:
            return
        if l <= segl and segr <= r:
            self.apply_min(node, segl, segr, x)
            return
        self.push(node, segl, segr)
        mid = (segl + segr) // 2
        if l <= mid:
            self.update_min_range(l, r, x, 2*node+1, segl, mid)
        if r > mid:
            self.update_min_range(l, r, x, 2*node+2, mid+1, segr)
        self.t_min[node] = (self.t_min[2*node+1] + self.t_min[2*node+2]) % MOD
        self.t_max[node] = (self.t_max[2*node+1] + self.t_max[2*node+2]) % MOD
        self.t_prod[node] = (self.t_prod[2*node+1] + self.t_prod[2*node+2]) % MOD
        
    def update_max_range(self, l, r, x, node=0, segl=0, segr=None):
        if segr is None:
            segr = self.size - 1
        if r < segl or segr < l:
            return
        if l <= segl and segr <= r:
            self.apply_max(node, segl, segr, x)
            return
        self.push(node, segl, segr)
        mid = (segl + segr) // 2
        if l <= mid:
            self.update_max_range(l, r, x, 2*node+1, segl, mid)
        if r > mid:
            self.update_max_range(l, r, x, 2*node+2, mid+1, segr)
        self.t_min[node] = (self.t_min[2*node+1] + self.t_min[2*node+2]) % MOD
        self.t_max[node] = (self.t_max[2*node+1] + self.t_max[2*node+2]) % MOD
        self.t_prod[node] = (self.t_prod[2*node+1] + self.t_prod[2*node+2]) % MOD
        
    def set_point(self, idx, min_val, max_val, node=0, segl=0, segr=None):
        if segr is None:
            segr = self.size - 1
        if segl == segr:
            if idx == segl:
                self.t_min[node] = min_val % MOD
                self.t_max[node] = max_val % MOD
                self.t_prod[node] = (min_val * max_val) % MOD
                self.lazy_min_set[node] = False
                self.lazy_max_set[node] = False
            return
        self.push(node, segl, segr)
        mid = (segl + segr) // 2
        if idx <= mid:
            self.set_point(idx, min_val, max_val, 2*node+1, segl, mid)
        else:
            self.set_point(idx, min_val, max_val, 2*node+2, mid+1, segr)
        self.t_min[node] = (self.t_min[2*node+1] + self.t_min[2*node+2]) % MOD
        self.t_max[node] = (self.t_max[2*node+1] + self.t_max[2*node+2]) % MOD
        self.t_prod[node] = (self.t_prod[2*node+1] + self.t_prod[2*node+2]) % MOD
        
    def query(self, l, r, node=0, segl=0, segr=None):
        if segr is None:
            segr = self.size - 1
        if r < segl or segr < l:
            return 0
        if l <= segl and segr <= r:
            return self.t_prod[node]
        self.push(node, segl, segr)
        mid = (segl + segr) // 2
        left_res = self.query(l, r, 2*node+1, segl, mid)
        right_res = self.query(l, r, 2*node+2, mid+1, segr)
        return (left_res + right_res) % MOD

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    a = list(map(int, data[1:1+n]))
    
    if n == 0:
        print(0)
        return
        
    seg_tree = SegmentTree(n)
    stack_min = []
    stack_max = []
    total = 0

    for i in range(n):
        while stack_min and a[stack_min[-1]] >= a[i]:
            stack_min.pop()
        j1 = stack_min[-1] if stack_min else -1

        while stack_max and a[stack_max[-1]] <= a[i]:
            stack_max.pop()
        j2 = stack_max[-1] if stack_max else -1

        if j1 + 1 <= i - 1:
            seg_tree.update_min_range(j1+1, i-1, a[i])
        if j2 + 1 <= i - 1:
            seg_tree.update_max_range(j2+1, i-1, a[i])
            
        seg_tree.set_point(i, a[i], a[i])
        F_i = seg_tree.query(0, i)
        total = (total + F_i) % MOD
        
        stack_min.append(i)
        stack_max.append(i)
        
    print(total % MOD)

if __name__ == '__main__':
    main()