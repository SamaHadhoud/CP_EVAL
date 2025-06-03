import sys
from bisect import bisect_left, bisect_right

class SegmentTree:
    def __init__(self, n):
        self.n = n
        self.size = 1
        while self.size < n:
            self.size *= 2
        self.first_active = [10**18] * (2 * self.size)
        self.last_active = [-1] * (2 * self.size)
    
    def build(self, active):
        for i in range(self.n):
            self.first_active[self.size + i] = i if active[i] else 10**18
            self.last_active[self.size + i] = i if active[i] else -1
        for i in range(self.n, self.size):
            self.first_active[self.size + i] = 10**18
            self.last_active[self.size + i] = -1
        for i in range(self.size-1, 0, -1):
            self.first_active[i] = min(self.first_active[2*i], self.first_active[2*i+1])
            self.last_active[i] = max(self.last_active[2*i], self.last_active[2*i+1])
    
    def update(self, i, active):
        idx = self.size + i
        if active:
            self.first_active[idx] = i
            self.last_active[idx] = i
        else:
            self.first_active[idx] = 10**18
            self.last_active[idx] = -1
        idx //= 2
        while idx:
            self.first_active[idx] = min(self.first_active[2*idx], self.first_active[2*idx+1])
            self.last_active[idx] = max(self.last_active[2*idx], self.last_active[2*idx+1])
            idx //= 2

    def query_first_active(self, l, r):
        l += self.size
        r += self.size
        res = 10**18
        while l <= r:
            if l % 2 == 1:
                res = min(res, self.first_active[l])
                l += 1
            if r % 2 == 0:
                res = min(res, self.first_active[r])
                r -= 1
            l //= 2
            r //= 2
        return res if res != 10**18 else -1

    def query_last_active(self, l, r):
        l += self.size
        r += self.size
        res = -1
        while l <= r:
            if l % 2 == 1:
                res = max(res, self.last_active[l])
                l += 1
            if r % 2 == 0:
                res = max(res, self.last_active[r])
                r -= 1
            l //= 2
            r //= 2
        return res

def can(X, A_orig):
    n = len(A_orig)
    if n == 0:
        return True
    A = sorted(A_orig)
    candidate_starts = set()
    candidate_starts.add(A[0])
    candidate_starts.add(A[-1])
    mid_index = n // 2
    candidate_starts.add(A[mid_index])
    if mid_index - 1 >= 0:
        candidate_starts.add(A[mid_index - 1])
    if mid_index + 1 < n:
        candidate_starts.add(A[mid_index + 1])
    
    clusters = []
    i = 0
    max_cluster_size = 0
    best_start = 0
    best_end = 0
    while i < n:
        j = i
        while j + 1 < n and A[j+1] - A[j] < X:
            j += 1
        current_size = j - i + 1
        if current_size > max_cluster_size:
            max_cluster_size = current_size
            best_start = i
            best_end = j
        i = j + 1
    
    if max_cluster_size > 0:
        candidate_starts.add(A[best_start])
        candidate_starts.add(A[best_end])
    
    for start_val in candidate_starts:
        active = [True] * n
        seg_tree = SegmentTree(n)
        seg_tree.build(active)
        
        start_indices = [i for i in range(n) if A[i] == start_val and active[i]]
        if not start_indices:
            continue
        start_idx = start_indices[0]
        active[start_idx] = False
        seg_tree.update(start_idx, False)
        last = A[start_idx]
        count = 1
        
        for _ in range(n-1):
            next_candidate_idx = -1
            target_high = last + X
            low_bound = bisect_left(A, target_high)
            if low_bound < n:
                idx1 = seg_tree.query_first_active(low_bound, n-1)
                if idx1 != -1 and A[idx1] >= target_high:
                    next_candidate_idx = idx1
            if next_candidate_idx == -1:
                target_low = last - X
                high_bound = bisect_right(A, target_low) - 1
                if high_bound >= 0:
                    idx2 = seg_tree.query_last_active(0, high_bound)
                    if idx2 != -1 and A[idx2] <= target_low:
                        next_candidate_idx = idx2
            if next_candidate_idx == -1:
                break
            active[next_candidate_idx] = False
            seg_tree.update(next_candidate_idx, False)
            last = A[next_candidate_idx]
            count += 1
        
        if count == n:
            return True
    return False

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    H = list(map(int, data[1:1+n]))
    lo = 0
    hi = 10**9
    ans = 0
    while lo <= hi:
        mid = (lo + hi) // 2
        if can(mid, H):
            ans = mid
            lo = mid + 1
        else:
            hi = mid - 1
    print(ans)

if __name__ == '__main__':
    main()