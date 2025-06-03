import bisect

def find_code_geq(S, t, K, pos_map):
    n = len(S)
    if S[0] < t[0]:
        return None
    res = [S[0]]
    current_index = 0
    bound = (S[0] == t[0])
    
    for j in range(1, K):
        L = current_index + 1
        R = n - (K - j)
        if L > R:
            return None
        
        if bound:
            low_bound = t[j]
        else:
            low_bound = 'A'
        
        candidate_char = None
        candidate_index = None
        for c_code in range(ord(low_bound), ord('Z') + 1):
            c_char = chr(c_code)
            if c_char not in pos_map:
                continue
            arr = pos_map[c_char]
            pos_in_arr = bisect.bisect_left(arr, L)
            if pos_in_arr < len(arr) and arr[pos_in_arr] <= R:
                candidate_char = c_char
                candidate_index = arr[pos_in_arr]
                break
        
        if candidate_char is None:
            return None
        
        res.append(candidate_char)
        current_index = candidate_index
        
        if bound:
            if candidate_char > t[j]:
                bound = False
                
    return ''.join(res)

def greedy_suffix(S, start, length, pos_map, n):
    if length == 0:
        return ""
    res = []
    current = start
    for j in range(length):
        R_bound = n - (length - j)
        found_char = None
        found_index = None
        for c_code in range(ord('A'), ord('Z') + 1):
            c_char = chr(c_code)
            if c_char not in pos_map:
                continue
            arr = pos_map[c_char]
            pos_in_arr = bisect.bisect_left(arr, current)
            if pos_in_arr < len(arr) and arr[pos_in_arr] <= R_bound:
                found_char = c_char
                found_index = arr[pos_in_arr]
                break
        if found_char is None:
            return None
        res.append(found_char)
        current = found_index + 1
    return ''.join(res)

def next_code_helper(S, t, K, pos_map):
    n = len(S)
    occ = []
    current_index = -1
    for i in range(K):
        c = t[i]
        if c not in pos_map:
            return None
        arr = pos_map[c]
        pos_in_arr = bisect.bisect_right(arr, current_index)
        if pos_in_arr >= len(arr):
            return None
        occ_val = arr[pos_in_arr]
        occ.append(occ_val)
        current_index = occ_val

    for i in range(K - 1, 0, -1):
        L = occ[i - 1] + 1
        R = n - (K - i)
        if L > R:
            continue
        candidate_char = None
        candidate_index = None
        for c_code in range(ord(t[i]) + 1, ord('Z') + 1):
            c_char = chr(c_code)
            if c_char not in pos_map:
                continue
            arr = pos_map[c_char]
            pos_in_arr = bisect.bisect_left(arr, L)
            if pos_in_arr < len(arr) and arr[pos_in_arr] <= R:
                candidate_char = c_char
                candidate_index = arr[pos_in_arr]
                break
        if candidate_char is None:
            continue
        suffix = greedy_suffix(S, candidate_index + 1, K - i - 1, pos_map, n)
        if suffix is None:
            continue
        new_code = t[:i] + candidate_char + suffix
        return new_code
    return None

def main():
    import sys
    data = sys.stdin.read().splitlines()
    if not data: 
        return
    n_countries, K_val = map(int, data[0].split())
    country_names = []
    for i in range(1, 1 + n_countries):
        country_names.append(data[i].strip())
    
    pos_maps = {}
    for name in country_names:
        d = {}
        for idx, char in enumerate(name):
            if char not in d:
                d[char] = []
            d[char].append(idx)
        pos_maps[name] = d
    
    sorted_countries = sorted([(name, idx) for idx, name in enumerate(country_names)], key=lambda x: x[0])
    
    codes = [None] * n_countries
    prev_code = "A" * K_val
    
    for i in range(n_countries):
        name, orig_idx = sorted_countries[i]
        if i == 0:
            candidate = find_code_geq(name, prev_code, K_val, pos_maps[name])
            if candidate is None:
                print("NO")
                return
            codes[orig_idx] = candidate
            prev_code = candidate
        else:
            candidate = find_code_geq(name, prev_code, K_val, pos_maps[name])
            if candidate is None:
                print("NO")
                return
            if candidate > prev_code:
                codes[orig_idx] = candidate
                prev_code = candidate
            else:
                next_candidate = next_code_helper(name, prev_code, K_val, pos_maps[name])
                if next_candidate is None:
                    print("NO")
                    return
                codes[orig_idx] = next_candidate
                prev_code = next_candidate
                
    print("YES")
    for code in codes:
        print(code)

if __name__ == "__main__":
    main()