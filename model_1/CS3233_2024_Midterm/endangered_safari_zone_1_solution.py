import sys
import math

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    it = iter(data)
    n = int(next(it)); m_val = int(next(it)); K_val = int(next(it))
    arr0 = [int(next(it)) - 1 for _ in range(n)]
    
    queries = []
    updates = []
    current_arr = arr0[:]
    for i in range(m_val):
        q_type = int(next(it)); a = int(next(it)); b = int(next(it))
        if q_type == 1:
            pos = a - 1
            new_val = b - 1
            old_val = current_arr[pos]
            updates.append((pos, old_val, new_val))
            current_arr[pos] = new_val
        else:
            l = a - 1; r = b - 1
            time_stamp = len(updates)
            queries.append((time_stamp, l, r, len(queries)))
    
    if not queries:
        return
    
    block_size = int(round(n ** (2.0/3.0)))
    if block_size < 1:
        block_size = 1
    
    def mo_cmp(query):
        time, l, r, idx = query
        l_block = l // block_size
        r_block = r // block_size
        return (l_block, r_block if l_block % 2 == 0 else -r_block, time)
    
    queries_sorted = sorted(queries, key=mo_cmp)
    
    arr = current_arr[:]
    freq = [0] * (n + 1)
    distinct_count = 0
    bad_count = 0
    
    def add_element(i):
        nonlocal distinct_count, bad_count
        x = arr[i]
        if freq[x] == 0:
            distinct_count += 1
        freq[x] += 1
        if freq[x] == K_val + 1:
            bad_count += 1
    
    def remove_element(i):
        nonlocal distinct_count, bad_count
        x = arr[i]
        if freq[x] == K_val + 1:
            bad_count -= 1
        freq[x] -= 1
        if freq[x] == 0:
            distinct_count -= 1
    
    def apply_update(update, current_l, current_r):
        nonlocal arr
        pos, old_val, new_val = update
        if current_l <= pos <= current_r:
            remove_element(pos)
        arr[pos] = new_val
        if current_l <= pos <= current_r:
            add_element(pos)
    
    def unapply_update(update, current_l, current_r):
        nonlocal arr
        pos, old_val, new_val = update
        if current_l <= pos <= current_r:
            remove_element(pos)
        arr[pos] = old_val
        if current_l <= pos <= current_r:
            add_element(pos)
    
    current_l = 0
    current_r = -1
    current_time = len(updates)
    ans_arr = [0] * len(queries)
    
    for query in queries_sorted:
        time_stamp, l, r, idx = query
        while current_time < time_stamp:
            apply_update(updates[current_time], current_l, current_r)
            current_time += 1
        while current_time > time_stamp:
            current_time -= 1
            unapply_update(updates[current_time], current_l, current_r)
        
        while current_r < r:
            current_r += 1
            add_element(current_r)
        while current_r > r:
            remove_element(current_r)
            current_r -= 1
        while current_l < l:
            remove_element(current_l)
            current_l += 1
        while current_l > l:
            current_l -= 1
            add_element(current_l)
        
        ans_arr[idx] = distinct_count - bad_count
    
    for ans in ans_arr:
        print(ans)

if __name__ == '__main__':
    main()