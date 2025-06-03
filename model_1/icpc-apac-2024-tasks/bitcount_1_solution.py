import sys

def main():
    data = sys.stdin.read().split()
    t = int(data[0])
    index = 1
    results = []
    for _ in range(t):
        n = int(data[index]); index += 1
        a = list(map(int, data[index:index+n]))
        index += n
        
        valid = True
        if n > 1:
            for i in range(n-1):
                d = a[i+1] - a[i]
                if d > 1:
                    valid = False
                    break
                elif d <= 0:
                    k = 1 - d
                    if k > a[i]:
                        valid = False
                        break
        if not valid:
            results.append("-1")
            continue
            
        mod = 1
        res = 0
        if n > 1:
            for i in range(n-1):
                if a[i+1] - a[i]] == 1:
                    k_i = 0
                else:
                    k_i = 1 - (a[i+1] - a[i]])
                m_i = 1 << (k_i + 1)
                base_val = (1 << k_i) - 1 - i
                r_i = base_val % m_i
                if r_i < 0:
                    r_i += m_i
                    
                g = min(mod, m_i)
                if (res % g) != (r_i % g):
                    valid = False
                    break
                    
                if mod <= m_i:
                    diff = r_i - res
                    if diff % mod != 0:
                        valid = False
                        break
                    quotient = diff // mod
                    step_size = m_i // mod
                    t0 = quotient % step_size
                    res = res + t0 * mod
                    mod = m_i
                    res %= mod
            if not valid:
                results.append("-1")
                continue
                
        v0 = None
        v1 = None
        valid_sequence = True
        for i in range(n):
            base_val = res + i
            if base_val < mod:
                s = bin(base_val)
                pc = s.count('1')
                v_val = a[i] - pc
                if v_val < 0:
                    valid_sequence = False
                    break
                if v0 is None:
                    v0 = v_val
                else:
                    if v_val != v0:
                        valid_sequence = False
                        break
            else:
                base_val2 = base_val - mod
                s = bin(base_val2)
                pc = s.count('1')
                v_val = a[i] - pc
                if v_val < 0:
                    valid_sequence = False
                    break
                if v1 is None:
                    v1 = v_val
                else:
                    if v_val != v1:
                        valid_sequence = False
                        break
                        
        if not valid_sequence:
            results.append("-1")
            continue
            
        if v0 is not None and v1 is not None:
            if not (1 <= v1 <= v0 + 1):
                results.append("-1")
                continue
        if v1 is not None and v0 is None:
            if v1 < 1:
                results.append("-1")
                continue
                
        if v0 is not None and v1 is None:
            if v0 == 0:
                t_min = 0
            else:
                t_min = (1 << v0) - 1
        elif v0 is None and v1 is not None:
            t_min = (1 << v1) - 2
        else:
            if v1 == v0 + 1:
                t_min = (1 << (v0+1)) - 2
            else:
                k = v0 - v1 + 1
                if v1 == 0:
                    t_min = 0
                else:
                    part1 = (1 << (v1-1)) - 1
                    part1_shifted = part1 << (k+1)
                    part2 = (1 << k) - 1
                    t_min = part1_shifted | part2
                    
        x_candidate = t_min * mod + res
        results.append(str(x_candidate))
        
    print("\n".join(results))

if __name__ == '__main__':
    main()