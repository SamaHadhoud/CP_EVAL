import sys
from collections import deque
import array

sys.setrecursionlimit(300000)

class WaveletTree:
    def __init__(self, arr):
        self.arr = arr
        n_val = len(arr)
        self.n = n_val
        self.min_val = min(arr)
        self.max_val = max(arr)
        self.tree = []
        self.lo = []
        self.hi = []
        self.left = []
        self.right = []
        if self.min_val == self.max_val:
            return
        self._build(0, n_val, self.min_val, self.max_val)
    
    def _build(self, l, r, low, high):
        if low >= high or l >= r:
            return
        mid = (low + high) // 2
        tree_node = array('I')
        left_count = 0
        for i in range(l, r):
            if self.arr[i] <= mid:
                left_count += 1
        tree_node.append(left_count)
        left_count_so_far = left_count
        for i in range(l, r):
            if self.arr[i] <= mid:
                left_count_so_far -= 1
            tree_node.append(left_count_so_far)
        self.tree.append(tree_node)
        self.lo.append(low)
        self.hi.append(high)
        left_idx = len(self.left)
        self.left.append(-1)
        self.right.append(-1)
        arr2 = array('I', [0] * (r - l))
        idx_left = 0
        idx_right = tree_node[0]
        for i in range(l, r):
            if self.arr[i] <= mid:
                arr2[idx_left] = self.arr[i]
                idx_left += 1
            else:
                arr2[idx_right] = self.arr[i]
                idx_right += 1
        for i in range(l, r):
            self.arr[i] = arr2[i - l]
        self.left[left_idx] = len(self.left)
        self._build(l, l + tree_node[0], low, mid)
        self.right[left_idx] = len(self.left)
        self._build(l + tree_node[0], r, mid + 1, high)

    def _range_query_leq(self, node_idx, l, r, x):
        if node_idx == -1 or l >= r or self.lo[node_idx] > x:
            return 0
        if self.hi[node_idx] <= x:
            return r - l
        mid = (self.lo[node_idx] + self.hi[node_idx]) // 2
        left_node = self.left[node_idx]
        right_node = self.right[node_idx]
        if l == 0:
            count_left = 0
        else:
            count_left = self.tree[node_idx][l]
        left_total = self.tree[node_idx][l + r] - count_left
        if x <= mid:
            if left_node == -1:
                return 0
            return self._range_query_leq(left_node, l - (self.tree[node_idx][l] - count_left), r - (self.tree[node_idx][l + r] - count_left + left_total), x)
        else:
            left_count = left_total
            right_count = 0
            if left_node != -1:
                left_part = self._range_query_leq(left_node, l - (self.tree[node_idx][l] - count_left), l - (self.tree[node_idx][l] - count_left) + left_count, mid)
            if right_node != -1:
                right_part = self._range_query_leq(right_node, (l - (self.tree[node_idx][l] - count_left)) + left_count, (r - (self.tree[node_idx][l + r] - count_left)) - (l - (self.tree[node_idx][l] - count_left)) - left_count, x)
                right_count = right_part
            return left_count + right_count

    def range_query_leq(self, l, r, x):
        if l > r:
            return 0
        if self.min_val == self.max_val:
            if self.min_val <= x:
                return r - l + 1
            else:
                return 0
        return self._range_query_leq(0, l, r + 1, x)

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    n = int(data[0])
    q = int(data[1])
    p_list = list(map(int, data[2:2 + n]))
    parent_arr = [0] * (n + 1)
    children = [[] for _ in range(n + 1)]
    root = None
    for i in range(1, n + 1):
        p_i = p_list[i - 1]
        parent_arr[i] = p_i
        if p_i == 0:
            root = i
        else:
            children[p_i].append(i)
    if root is None:
        for i in range(1, n + 1):
            if parent_arr[i] == 0:
                root = i
                break
    is_chain = True
    if n > 1:
        if len(children[root]) != 1:
            is_chain = False
        else:
            for i in range(1, n + 1):
                if i == root:
                    continue
                if len(children[i]) > 1:
                    is_chain = False
                    break
    else:
        is_chain = True
    if is_chain:
        queries = list(map(int, data[2 + n:2 + n + q]))
        answers = []
        for kj in queries:
            x0 = (kj - 1) // n + 1
            r = (kj - 1) % n + 1
            if r <= x0 - 1:
                inner_val = (r - 1) * (n + 1)
            else:
                inner_val = (x0 - 1) * n + (r - 1)
            ans = (x0 - 1) * n * n + inner_val
            answers.append(str(ans))
        sys.stdout.write("\n".join(answers))
        return
    in_time = [0] * (n + 1)
    out_time = [0] * (n + 1)
    timer = 0
    A_arr = array('I', [0] * n)
    stack = [root]
    parent_arr[0] = 0
    while stack:
        u = stack.pop()
        if u > 0:
            in_time[u] = timer
            A_arr[timer] = u
            timer += 1
            stack.append(-u)
            for v in reversed(children[u]):
                stack.append(v)
        else:
            u = -u
            out_time[u] = timer - 1
    wavelet = WaveletTree(A_arr)
    cache = {}
    queries = list(map(int, data[2 + n:2 + n + q]))
    answers = []
    for kj in queries:
        x0 = (kj - 1) // n + 1
        r = (kj - 1) % n + 1
        if x0 in cache:
            segments, sizes, prefix_sum, next_child_dict = cache[x0]
        else:
            path_list = []
            cur = x0
            while cur != 0:
                path_list.append(cur)
                cur = parent_arr[cur]
            path_list.reverse()
            next_child_dict = {}
            Lp = len(path_list)
            for i in range(Lp - 1):
                next_child_dict[path_list[i]] = path_list[i + 1]
            segments = sorted(path_list, key=lambda z: (z - 1) * n)
            sizes = []
            for z in segments:
                if z in next_child_dict:
                    c = next_child_dict[z]
                else:
                    c = None
                if c is None:
                    size_z = out_time[z] - in_time[z] + 1
                else:
                    part1 = in_time[c] - in_time[z]
                    part2 = out_time[z] - out_time[c]
                    size_z = part1 + part2
                sizes.append(size_z)
            prefix_sum = []
            s = 0
            for sz in sizes:
                s += sz
                prefix_sum.append(s)
            cache[x0] = (segments, sizes, prefix_sum, next_child_dict)
        total_segments = len(prefix_sum)
        if total_segments == 0:
            ans_val = (x0 - 1) * n * n
            answers.append(str(ans_val))
            continue
        lo, hi = 0, total_segments - 1
        idx = -1
        if r <= prefix_sum[0]:
            idx = 0
        else:
            lo, hi = 0, total_segments - 1
            while lo < hi:
                mid = (lo + hi) // 2
                if prefix_sum[mid] < r:
                    lo = mid + 1
                else:
                    hi = mid
            idx = lo
        z = segments[idx]
        prev_sum = prefix_sum[idx - 1] if idx > 0 else 0
        k0 = r - prev_sum
        if z in next_child_dict:
            c = next_child_dict[z]
        else:
            c = None
        intervals = []
        if c is None:
            L1 = in_time[z]
            R1 = out_time[z]
            if L1 <= R1:
                intervals.append((L1, R1))
        else:
            L1 = in_time[z]
            R1 = in_time[c] - 1
            if L1 <= R1:
                intervals.append((L1, R1))
            L2 = out_time[c] + 1
            R2 = out_time[z]
            if L2 <= R2:
                intervals.append((L2, R2))
        base_z = (z - 1) * n
        lowY = 1
        highY = n
        while lowY < highY:
            midY = (lowY + highY) // 2
            cnt = 0
            for (L, R) in intervals:
                if L <= R:
                    cnt += wavelet.range_query_leq(L, R, midY)
            if cnt >= k0:
                highY = midY
            else:
                lowY = midY + 1
        Y0 = lowY
        inner_value = base_z + (Y0 - 1)
        ans_val = (x0 - 1) * n * n + inner_value
        answers.append(str(ans_val))
    sys.stdout.write("\n".join(answers))

if __name__ == "__main__":
    main()