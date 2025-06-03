import sys

def main():
    data = sys.stdin.read().splitlines()
    n = int(data[0])
    total_contestants = 0
    total_zeros = 0
    total_ones = 0
    saving0 = 0
    has0 = False
    has1 = False
    list_force0 = []
    list_force1 = []
    
    index = 1
    for i in range(n):
        s = data[index].strip()
        index += 1
        L = len(s)
        total_contestants += L
        count0 = s.count('0')
        count1 = L - count0
        total_zeros += count0
        total_ones += count1
        saving0 += max(count0, count1)
        
        if count0 >= count1:
            has0 = True
        if count1 >= count0:
            has1 = True
            
        if count0 < count1:
            loss0 = count1 - count0
            list_force0.append(loss0)
        if count1 < count0:
            loss1 = count0 - count1
            list_force1.append(loss1)
            
    if total_zeros > 0 and not has0:
        if list_force0:
            min_loss0 = min(list_force0)
            saving0 -= min_loss0
    if total_ones > 0 and not has1:
        if list_force1:
            min_loss1 = min(list_force1)
            saving0 -= min_loss1
            
    moves = total_contestants - saving0
    print(moves)

if __name__ == "__main__":
    main()