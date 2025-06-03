import sys

def main():
    data = sys.stdin.read().splitlines()
    n = int(data[0].strip())
    s = data[1].strip()
    
    total_A = s.count('A')
    total_C = s.count('C')
    total_seats = 2 * n
    
    if total_A < total_C:
        print("NO")
        return
        
    credit = total_A - total_C
    balance = 0
    for char in s:
        if char == 'A':
            balance += 1
        elif char == 'C':
            balance -= 1
            if balance < 0:
                if credit > 0:
                    credit -= 1
                    balance += 1
                else:
                    print("NO")
                    return
                    
    credit = total_A - total_C
    balance = 0
    for char in reversed(s):
        if char == 'A':
            balance += 1
        elif char == 'C':
            balance -= 1
            if balance < 0:
                if credit > 0:
                    credit -= 1
                    balance += 1
                else:
                    print("NO")
                    return
                    
    print("YES")
    res = []
    for _ in range(total_C):
        res.append('A')
        res.append('C')
        
    remaining_A = total_A - total_C
    remaining_seats = total_seats - len(res)
    res.extend(['A'] * remaining_A)
    res.extend(['.'] * (remaining_seats - remaining_A))
    
    print(''.join(res))

if __name__ == "__main__":
    main()