import sys

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    lands = []
    for i in range(n):
        L = int(data[1+2*i])
        W = int(data[1+2*i+1])
        long_i = max(L, W)
        short_i = min(L, W)
        lands.append((long_i, short_i))
    
    candidate_one = 0
    for long_i, short_i in lands:
        product = long_i * short_i
        if product > candidate_one:
            candidate_one = product
            
    lands.sort(key=lambda x: x[0], reverse=True)
    
    candidate_two = 0
    max_short = 0
    for long_i, short_i in lands:
        if max_short > 0:
            temp = min(short_i, max_short)
            candidate_ij = long_i * temp
            if candidate_ij > candidate_two:
                candidate_two = candidate_ij
        if short_i > max_short:
            max_short = short_i
            
    if candidate_one >= 2 * candidate_two:
        half_area = candidate_one // 2
        if candidate_one % 2 == 0:
            print(f"{half_area}.0")
        else:
            print(f"{half_area}.5")
    else:
        print(f"{candidate_two}.0")

if __name__ == "__main__":
    main()