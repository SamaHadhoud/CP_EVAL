import sys
import random

sys.setrecursionlimit(500000)

class Node:
    __slots__ = ('key', 'priority', 'left', 'right')
    def __init__(self, key):
        self.key = key
        self.priority = random.random()
        self.left = None
        self.right = None

def split(node, key):
    if node is None:
        return (None, None)
    if node.key <= key:
        l, r = split(node.right, key)
        node.right = l
        return (node, r)
    else:
        l, r = split(node.left, key)
        node.left = r
        return (l, node)

def merge(left, right):
    if left is None:
        return right
    if right is None:
        return left
    if left.priority > right.priority:
        left.right = merge(left.right, right)
        return left
    else:
        right.left = merge(left, right.left)
        return right

def insert_into_treap(root, key):
    l, r = split(root, key-1)
    new_node = Node(key)
    return merge(merge(l, new_node), r)

def delete_from_treap(root, key):
    l, r = split(root, key-1)
    m, r = split(r, key)
    return merge(l, r)

def iterative_predecessor(root, key):
    cur = root
    candidate = None
    while cur:
        if cur.key < key:
            candidate = cur.key
            cur = cur.right
        else:
            cur = cur.left
    return candidate

def iterative_successor(root, key):
    cur = root
    candidate = None
    while cur:
        if cur.key > key:
            candidate = cur.key
            cur = cur.left
        else:
            cur = cur.right
    return candidate

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    n = int(next(it)); m = int(next(it))
    dislike_set = set()
    for _ in range(m):
        x = int(next(it)); y = int(next(it))
        if x > y:
            x, y = y, x
        dislike_set.add((x, y))
    
    q = int(next(it))
    queries = []
    for _ in range(q):
        d = int(next(it)); z = int(next(it))
        queries.append((d, z))
    
    total_discontent = 0
    root = None
    left_neighbor = {}
    right_neighbor = {}
    
    def get_dc(x):
        p_prev = left_neighbor.get(x, None)
        p_next = right_neighbor.get(x, None)
        if p_prev is not None and p_next is not None:
            a = (min(x, p_prev), max(x, p_prev))
            b = (min(x, p_next), max(x, p_next))
            d_prev = a in dislike_set
            d_next = b in dislike_set
            if d_prev and d_next:
                return 3233
            elif d_prev:
                return 323
            elif d_next:
                return 32
            else:
                return 3
        elif p_prev is not None:
            a = (min(x, p_prev), max(x, p_prev))
            d_prev = a in dislike_set
            if d_prev:
                return 323
            else:
                return 3
        elif p_next is not None:
            b = (min(x, p_next), max(x, p_next))
            d_next = b in dislike_set
            if d_next:
                return 32
            else:
                return 3
        else:
            return 3
    
    out_lines = []
    for d, z in queries:
        if d == 1:
            p = iterative_predecessor(root, z) if root is not None else None
            s = iterative_successor(root, z) if root is not None else None
            if p is not None:
                total_discontent -= get_dc(p)
            if s is not None:
                total_discontent -= get_dc(s)
            left_neighbor[z] = p
            right_neighbor[z] = s
            if p is not None:
                right_neighbor[p] = z
            if s is not None:
                left_neighbor[s] = z
            root = insert_into_treap(root, z)
            dc_p = get_dc(p) if p is not None else 0
            dc_z = get_dc(z)
            dc_s = get_dc(s) if s is not None else 0
            total_discontent += dc_p + dc_z + dc_s
        else:
            p = left_neighbor.get(z, None)
            s = right_neighbor.get(z, None)
            to_remove = 0
            if p is not None:
                to_remove += get_dc(p)
            to_remove += get_dc(z)
            if s is not None:
                to_remove += get_dc(s)
            total_discontent -= to_remove
            if p is not None:
                right_neighbor[p] = s
            if s is not None:
                left_neighbor[s] = p
            root = delete_from_treap(root, z)
            to_add = 0
            if p is not None:
                to_add += get_dc(p)
            if s is not None:
                to_add += get_dc(s)
            total_discontent += to_add
        out_lines.append(str(total_discontent))
    
    sys.stdout.write("\n".join(out_lines))

if __name__ == '__main__':
    main()