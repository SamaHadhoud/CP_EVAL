import heapq
import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    k = int(data[1])
    l_val = int(data[2])
    a = list(map(int, data[3:3+n]))
    
    a.sort(reverse=True)
    base = sum(a[:k])
    res = [base]
    
    if k < n:
        heap = []
        seen = set()
        for i in range(k-1, -1, -1):
            new_sum = base - a[i] + a[k]
            state = (i, k, 1)
            heapq.heappush(heap, (-new_sum, i, k, 1))
        
        while len(res) < l_val and heap:
            neg_val, i, j, depth = heapq.heappop(heap)
            if (i, j, depth) in seen:
                continue
            seen.add((i, j, depth))
            current_sum = -neg_val
            res.append(current_sum)
            if len(res) == l_val:
                break
                
            if j + 1 < n:
                new_sum1 = current_sum - a[j] + a[j+1]
                new_state1 = (i, j+1, depth)
                heapq.heappush(heap, (-new_sum1, i, j+1, depth))
                
            if i - 1 >= 0 and j + 1 < n:
                new_sum2 = current_sum - a[i-1] + a[j+1]
                new_state2 = (i-1, j+1, depth+1)
                heapq.heappush(heap, (-new_sum2, i-1, j+1, depth+1))
                
    for i in range(l_val):
        if i < len(res):
            print(res[i])
        else:
            print(-1)

if __name__ == '__main__':
    main()