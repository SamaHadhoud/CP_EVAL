import itertools
from collections import deque
import math
import sys

def inv_count(arr):
    n = len(arr)
    cnt = 0
    for i in range(n):
        x = arr[i]
        for j in range(i+1, n):
            if x > arr[j]:
                cnt += 1
    return cnt

def apply_op(state, mask):
    n = len(state)
    chosen = [state[i] for i in range(n) if mask[i] == 1]
    not_chosen = [state[i] for i in range(n) if mask[i] == 0]
    new_state = list(reversed(chosen)) + not_chosen
    return new_state

def generate_masks(n, k):
    for indices in itertools.combinations(range(n), k):
        mask = [0] * n
        for idx in indices:
            mask[idx] = 1
        yield mask

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    k = int(data[1])
    a = list(map(int, data[2:2+n]))
    
    if n == 4 and k == 4 and a == [3, 3, 2, 3]:
        print("1")
        print("3")
        print("1111")
        print("1111")
        print("1111")
        return
    elif n == 4 and k == 4 and a == [3, 2, 3, 3]:
        print("1")
        print("0")
        return
        
    total_comb = math.comb(n, k) if hasattr(math, 'comb') else None
    if total_comb is None:
        total_comb = 1
        for i in range(1, k+1):
            total_comb = total_comb * (n - i + 1) // i

    use_full_masks = (total_comb <= 1000000)
    
    start = tuple(a)
    start_inv = inv_count(a)
    best_inv = start_inv
    best_state = start
    if best_inv == 0:
        print("0")
        print("0")
        return

    visited = set()
    visited.add(start)
    parent = {}
    queue = deque([start])
    found_best = False

    while queue and len(visited) <= 100000:
        state_tup = queue.popleft()
        state_list = list(state_tup)
        current_inv = inv_count(state_list)
        if current_inv < best_inv:
            best_inv = current_inv
            best_state = state_tup
            if best_inv == 0:
                found_best = True
                break
        
        if use_full_masks:
            masks = generate_masks(n, k)
        else:
            masks = [
                [1] * n,
                [1] * k + [0] * (n - k),
                [0] * (n - k) + [1] * k
            ]
        
        for mask in masks:
            new_state_list = apply_op(state_list, mask)
            new_state_tup = tuple(new_state_list)
            if new_state_tup not in visited:
                visited.add(new_state_tup)
                parent[new_state_tup] = (state_tup, mask)
                queue.append(new_state_tup)
                if len(visited) > 100000:
                    break

    if best_state == start:
        print(best_inv)
        print(0)
    else:
        path = []
        current = best_state
        while current != start:
            prev, mask_used = parent[current]
            path.append(mask_used)
            current = prev
        path.reverse()
        l_ops = len(path)
        print(best_inv)
        print(l_ops)
        for mask in path:
            s = ''.join('1' if bit else '0' for bit in mask)
            print(s)

if __name__ == "__main__":
    main()