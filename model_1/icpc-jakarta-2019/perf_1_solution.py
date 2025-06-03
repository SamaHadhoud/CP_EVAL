import sys

class SegmentTree:
    def __init__(self, data, n):
        self.n = n
        self.size = 1
        while self.size < n:
            self.size *= 2
        self.tree = [10**18] * (2 * self.size)
        self.lazy = [0] * (2 * self.size)
        for i in range(n):
            self.tree[self.size + i] = data[i]
        for i in range(n, self.size):
            self.tree[self.size + i] = 10**18
        for i in range(self.size - 1, 0, -1):
            self.tree[i] = min(self.tree[2*i], self.tree[2*i+1])
    
    def apply(self, node, delta):
        self.tree[node] += delta
        if node < self.size:
            self.lazy[node] += delta
            
    def push(self, node):
        if self.lazy[node] != 0:
            self.apply(2*node, self.lazy[node])
            self.apply(2*node+1, self.lazy[node])
            self.lazy[node] = 0
            
    def update_range(self, l, r, delta, node, segL, segR):
        if r < segL or segR < l:
            return
        if l <= segL and segR <= r:
            self.apply(node, delta)
            return
        self.push(node)
        mid = (segL + segR) // 2
        self.update_range(l, r, delta, 2*node, segL, mid)
        self.update_range(l, r, delta, 2*node+1, mid+1, segR)
        self.tree[node] = min(self.tree[2*node], self.tree[2*node+1])
        
    def update(self, l, r, delta):
        if l > r:
            return
        self.update_range(l, r, delta, 1, 0, self.size-1)
        
    def global_min(self):
        return self.tree[1]

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    it = iter(data)
    n = int(next(it)); m = int(next(it)); q = int(next(it))
    A = [int(next(it)) for _ in range(n)]
    A0 = A[0]
    F1 = sum(1 for x in A if x < A0)
    
    R_arr = [0] * (m+1)
    B_arr = [None] * (m+1)
    G_current = [0] * (m+1)
    
    for i in range(1, m+1):
        R_i = int(next(it))
        R_arr[i] = R_i
        B_i = [int(next(it)) for _ in range(R_i)]
        B_arr[i] = B_i
        count_less = sum(1 for val in B_i if val < A0)
        G_current[i] = count_less
        
    if m > 0:
        H = [0] * (m+2)
        H[1] = F1
        D0 = [0] * m
        for t in range(1, m+1):
            D0[t-1] = H[t] - R_arr[t]
            H[t+1] = H[t] - R_arr[t] + G_current[t]
        seg_tree = SegmentTree(D0, m)
    else:
        seg_tree = None
        
    out_lines = []
    for _ in range(q):
        X = int(next(it)); Y = int(next(it)); Z = int(next(it))
        idx = Y - 1
        old_val = B_arr[X][idx]
        old_contrib = 1 if old_val < A0 else 0
        new_contrib = 1 if Z < A0 else 0
        delta = new_contrib - old_contrib
        
        if delta != 0:
            if m > 0 and X <= m-1:
                seg_tree.update(X, m-1, delta)
            B_arr[X][idx] = Z
            G_current[X] += delta
            
        if m == 0:
            out_lines.append("1")
        else:
            min_val = seg_tree.global_min()
            if min_val >= 0:
                out_lines.append("1")
            else:
                out_lines.append("0")
                
    sys.stdout.write("\n".join(out_lines) + "\n")

if __name__ == "__main__":
    main()