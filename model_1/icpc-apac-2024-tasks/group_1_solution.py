import math
import sys
import heapq

def main():
    maxN = 1000000
    divisors_list = [[] for _ in range(maxN + 1)]
    for i in range(1, maxN + 1):
        for j in range(i, maxN + 1, i):
            divisors_list[j].append(i)
    
    data = sys.stdin.read().split()
    t = int(data[0])
    index = 1
    out_lines = []
    for _ in range(t):
        n = int(data[index]); index += 1
        a1 = int(data[index]); index += 1
        arr = []
        need = n - 1
        while need > 0 and index < len(data):
            take = min(need, len(data) - index)
            arr.extend(map(int, data[index:index+take]))
            index += take
            need = n - 1 - len(arr)
        
        B = arr[:n-1] if len(arr) >= n-1 else arr + [0] * (n - 1 - len(arr))
        
        divisors = divisors_list[n]
        divisors = [k for k in divisors if k > 1]
        
        best_ratio_num = None
        best_ratio_den = None
        
        for k in divisors:
            A0 = [0] * k
            for i in range(len(B)):
                r = i % k
                A0[r] += B[i]
            
            G0 = [0] * k
            for j in range(k):
                prev_res = (j - 1) % k
                G0[j] = A0[prev_res]
            
            G0[0] += a1
            
            min_heap = []
            max_heap = []
            for j in range(k):
                heapq.heappush(min_heap, (G0[j], j))
                heapq.heappush(max_heap, (-G0[j], j))
            
            while min_heap and min_heap[0][0] != G0[min_heap[0][1]]:
                heapq.heappop(min_heap)
            while max_heap and -max_heap[0][0] != G0[max_heap[0][1]]:
                heapq.heappop(max_heap)
            if not min_heap or not max_heap:
                min_val = 10**18
                max_val = -10**18
            else:
                min_val = min_heap[0][0]
                max_val = -max_heap[0][0]
            best_ratio_this_k = (max_val, min_val)
            
            current_values = G0[:]
            
            for pos in range(1, n):
                residue_remove = (pos - 1) % k
                residue_add = pos % k
                
                old_val_remove = current_values[residue_remove]
                new_val_remove = old_val_remove - a1
                current_values[residue_remove] = new_val_remove
                heapq.heappush(min_heap, (new_val_remove, residue_remove))
                heapq.heappush(max_heap, (-new_val_remove, residue_remove))
                
                old_val_add = current_values[residue_add]
                new_val_add = old_val_add + a1
                current_values[residue_add] = new_val_add
                heapq.heappush(min_heap, (new_val_add, residue_add))
                heapq.heappush(max_heap, (-new_val_add, residue_add))
                
                base_index = pos - 1
                base_val = B[base_index]
                r0 = base_index % k
                old_val_r0 = current_values[r0]
                new_val_r0 = old_val_r0 + base_val
                current_values[r0] = new_val_r0
                heapq.heappush(min_heap, (new_val_r0, r0))
                heapq.heappush(max_heap, (-new_val_r0, r0))
                
                r1 = (r0 + 1) % k
                old_val_r1 = current_values[r1]
                new_val_r1 = old_val_r1 - base_val
                current_values[r1] = new_val_r1
                heapq.heappush(min_heap, (new_val_r1, r1))
                heapq.heappush(max_heap, (-new_val_r1, r1))
                
                while min_heap and min_heap[0][0] != current_values[min_heap[0][1]]:
                    heapq.heappop(min_heap)
                while max_heap and -max_heap[0][0] != current_values[max_heap[0][1]]:
                    heapq.heappop(max_heap)
                if not min_heap or not max_heap:
                    min_val_cur = 10**18
                    max_val_cur = -10**18
                else:
                    min_val_cur = min_heap[0][0]
                    max_val_cur = -max_heap[0][0]
                
                a_prev, b_prev = best_ratio_this_k
                if max_val_cur * b_prev < a_prev * min_val_cur:
                    best_ratio_this_k = (max_val_cur, min_val_cur)
            
            a_candidate, b_candidate = best_ratio_this_k
            if best_ratio_num is None:
                best_ratio_num = a_candidate
                best_ratio_den = b_candidate
            else:
                if a_candidate * best_ratio_den < best_ratio_num * b_candidate:
                    best_ratio_num = a_candidate
                    best_ratio_den = b_candidate
        
        g = math.gcd(best_ratio_num, best_ratio_den)
        p = best_ratio_num // g
        q = best_ratio_den // g
        out_lines.append(f"{p} {q}")
    
    print("\n".join(out_lines))

if __name__ == '__main__':
    main()