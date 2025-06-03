import sys
from collections import deque

MOD1 = 10**9 + 7
MOD2 = 10**9 + 9
BASE1 = 131
BASE2 = 131

def main():
    data = sys.stdin.read().split()
    if not data:
        return
    
    n = int(data[0])
    m = int(data[1])
    k = int(data[2])
    digits = list(map(int, data[3:3+m]))
    
    if n == 1:
        print(''.join(str(d) for d in digits[:k]))
        return
        
    base_power1 = pow(BASE1, n-2, MOD1) if n >= 2 else 0
    base_power2 = pow(BASE2, n-2, MOD2) if n >= 2 else 0
    
    init_digit = digits[0]
    initial_state = [init_digit] * (n - 1)
    current_state = deque(initial_state)
    
    H1 = 0
    H2 = 0
    for d in initial_state:
        H1 = (H1 * BASE1 + d) % MOD1
        H2 = (H2 * BASE2 + d) % MOD2
        
    s_list = [str(d) for d in initial_state]
    visited_pins = set()
    count = 0
    total_length = n - 1
    
    while count < k and total_length < 100000:
        found = False
        for d in digits:
            pin_hash1 = (H1 * BASE1 + d) % MOD1
            pin_hash2 = (H2 * BASE2 + d) % MOD2
            if (pin_hash1, pin_hash2) in visited_pins:
                continue
            visited_pins.add((pin_hash1, pin_hash2))
            count += 1
            s_list.append(str(d))
            front_digit = current_state.popleft()
            current_state.append(d)
            
            H1 = (H1 - (front_digit * base_power1)) % MOD1
            H1 = (H1 * BASE1 + d) % MOD1
            
            H2 = (H2 - (front_digit * base_power2)) % MOD2
            H2 = (H2 * BASE2 + d) % MOD2
            
            total_length += 1
            found = True
            break
            
        if not found and count < k:
            d = digits[-1]
            s_list.append(str(d))
            front_digit = current_state.popleft()
            current_state.append(d)
            
            H1 = (H1 - (front_digit * base_power1)) % MOD1
            H1 = (H1 * BASE1 + d) % MOD1
            
            H2 = (H2 - (front_digit * base_power2)) % MOD2
            H2 = (H2 * BASE2 + d) % MOD2
            
            total_length += 1
        if count >= k:
            break
            
    print(''.join(s_list))

if __name__ == '__main__':
    main()