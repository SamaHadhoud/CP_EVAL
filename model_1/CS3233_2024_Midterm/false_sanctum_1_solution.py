import sys
sys.setrecursionlimit(300000)

class Node:
    __slots__ = ('size', 'lc', 'rc', 'lrun', 'rrun', 'dens')
    def __init__(self, size, lc, rc, lrun, rrun, dens):
        self.size = size
        self.lc = lc
        self.rc = rc
        self.lrun = lrun
        self.rrun = rrun
        self.dens = dens

def merge(L, R):
    if L.size == 0:
        return R
    if R.size == 0:
        return L
    size = L.size + R.size
    lc = L.lc
    rc = R.rc
    lrun = L.lrun
    if L.lrun == L.size and L.rc == R.lc:
        lrun = L.lrun + R.lrun
    rrun = R.rrun
    if R.rrun == R.size and L.rc == R.lc:
        rrun = R.rrun + L.rrun
    dens = L.dens + R.dens
    if L.rc == R.lc:
        dens = dens - (L.rrun * L.rrun) - (R.lrun * R.lrun) + ((L.rrun + R.lrun) * (L.rrun + R.lrun))
    return Node(size, lc, rc, lrun, rrun, dens)

def build_tree(node, l, r, tree, s):
    if l == r:
        tree[node] = Node(1, s[l], s[l], 1, 1, 1)
        return
    mid = (l + r) // 2
    left_child = 2 * node + 1
    right_child = 2 * node + 2
    build_tree(left_child, l, mid, tree, s)
    build_tree(right_child, mid+1, r, tree, s)
    tree[node] = merge(tree[left_child], tree[right_child])

def update_tree(node, l, r, idx, c, tree):
    if l == r:
        tree[node] = Node(1, c, c, 1, 1, 1)
        return
    mid = (l + r) // 2
    left_child = 2 * node + 1
    right_child = 2 * node + 2
    if idx <= mid:
        update_tree(left_child, l, mid, idx, c, tree)
    else:
        update_tree(right_child, mid+1, r, idx, c, tree)
    tree[node] = merge(tree[left_child], tree[right_child])

def query_tree(node, l, r, ql, qr, tree):
    if ql <= l and r <= qr:
        return tree[node]
    mid = (l + r) // 2
    left_node = None
    right_node = None
    if ql <= mid:
        left_node = query_tree(2*node+1, l, mid, ql, min(qr, mid), tree)
    if qr > mid:
        right_node = query_tree(2*node+2, mid+1, r, max(ql, mid+1), qr, tree)
    if left_node is not None and right_node is not None:
        return merge(left_node, right_node)
    elif left_node is not None:
        return left_node
    elif right_node is not None:
        return right_node
    else:
        return Node(0, None, None, 0, 0, 0)

def main():
    data = sys.stdin.read().splitlines()
    if not data: 
        return
    n, q = map(int, data[0].split())
    s = data[1].strip()
    tree = [None] * (4 * n)
    build_tree(0, 0, n-1, tree, s)
    out_lines = []
    index = 2
    for i in range(q):
        parts = data[index].split()
        index += 1
        if parts[0] == '1':
            pos = int(parts[1]) - 1
            c = parts[2]
            update_tree(0, 0, n-1, pos, c, tree)
        else:
            l = int(parts[1]) - 1
            r = int(parts[2]) - 1
            node = query_tree(0, 0, n-1, l, r, tree)
            out_lines.append(str(node.dens))
    sys.stdout.write("\n".join(out_lines))

if __name__ == '__main__':
    main()