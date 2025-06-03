import sys

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    t = int(data[0])
    index = 1
    results = []
    for _ in range(t):
        n = int(data[index])
        index += 1
        w = list(map(int, data[index:index+n]))
        index += n
        l_list = list(map(int, data[index:index+n]))
        index += n
        
        total_wealth = sum(w)
        total_l = sum(l_list)
        l_list.sort()
        distinct = []
        freqs = []
        i = 0
        n_val = len(l_list)
        while i < n_val:
            j = i
            while j < n_val and l_list[j] == l_list[i]:
                j += 1
            distinct.append(l_list[i])
            freqs.append(j - i)
            i = j
        
        cum_freq = 0
        cum_sum = 0
        best = 0
        num_distinct = len(distinct)
        
        for j in range(num_distinct):
            d = distinct[j]
            S_d = total_l + d * cum_freq - cum_sum
            if S_d <= total_wealth:
                if d > best:
                    best = d
            
            count_here = freqs[j]
            cum_freq += count_here
            cum_sum += d * count_here
            
            num = total_wealth - total_l + cum_sum
            candidate_seg = num // cum_freq
            
            if j < num_distinct - 1:
                next_d = distinct[j+1]
                candidate_val = candidate_seg
                if candidate_val > next_d - 1:
                    candidate_val = next_d - 1
            else:
                candidate_val = candidate_seg
                
            if candidate_val > d:
                if candidate_val > best:
                    best = candidate_val
                    
        results.append(str(best))
    
    print("\n".join(results))

if __name__ == '__main__':
    main()