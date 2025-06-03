import sys

def solve_test_case(a, p_val, S, E):
    A_i = S.count('A')
    P_i = S.count('P')
    A_g = E.count('A')
    P_g = E.count('P')
    
    solutions = []
    for rA in range(0, 21):
        tempA = A_g - A_i + a * rA
        if tempA < 0 or tempA % 2 != 0:
            continue
        x = tempA // 2
        for rP in range(0, 21):
            tempP = P_g - P_i + p_val * rP
            if tempP < 0 or tempP % 2 != 0:
                continue
            y = tempP // 2
            total_ops = x + y + rA + rP
            solutions.append((total_ops, x, y, rA, rP))
    
    if not solutions:
        return (-1, None)
    
    solutions_sorted = sorted(solutions, key=lambda z: z[0])
    
    for sol in solutions_sorted:
        total_ops, x, y, rA, rP = sol
        current_string_list = list(S)
        operations = []
        valid = True
        
        for _ in range(x):
            pos = -1
            for idx in range(len(current_string_list)-1, -1, -1):
                if current_string_list[idx] == 'P':
                    pos = idx
                    break
            if pos == -1:
                for idx in range(len(current_string_list)):
                    if current_string_list[idx] == 'P':
                        pos = idx
                        break
                if pos == -1:
                    valid = False
                    break
            current_string_list = current_string_list[:pos] + ['A','P','A'] + current_string_list[pos+1:]
            operations.append(('+P', pos+1))
        
        if not valid:
            continue
            
        for _ in range(y):
            pos = -1
            for idx in range(len(current_string_list)-1, -1, -1):
                if current_string_list[idx] == 'A':
                    pos = idx
                    break
            if pos == -1:
                for idx in range(len(current_string_list)):
                    if current_string_list[idx] == 'A':
                        pos = idx
                        break
                if pos == -1:
                    valid = False
                    break
            current_string_list = current_string_list[:pos] + ['P','A','P'] + current_string_list[pos+1:]
            operations.append(('+A', pos+1))
        
        if not valid:
            continue
        
        current_str = ''.join(current_string_list)
        temp_operations = []
        temp_str = current_str
        
        for _ in range(rA):
            found_removal = False
            for start in range(0, len(temp_str) - a + 1):
                if temp_str[start:start+a] == 'A'*a:
                    temp_str = temp_str[:start] + temp_str[start+a:]
                    temp_operations.append(('-A', start+1))
                    found_removal = True
                    break
            if not found_removal:
                for start in range(len(temp_str)-a, -1, -1):
                    if temp_str[start:start+a] == 'A'*a:
                        temp_str = temp_str[:start] + temp_str[start+a:]
                        temp_operations.append(('-A', start+1))
                        found_removal = True
                        break
            if not found_removal:
                valid = False
                break
        
        if not valid:
            continue
        
        for _ in range(rP):
            found_removal = False
            for start in range(0, len(temp_str) - p