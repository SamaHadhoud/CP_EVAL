import math
from math import gcd
import sys

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    q = int(data[1])
    P = list(map(int, data[2:2+n]))
    queries = list(map(int, data[2+n:2+n+q]))
    
    P0 = [x - 1 for x in P]
    
    visited = [False] * n
    cycles = []
    for i in range(n):
        if not visited[i]:
            cycle = []
            cur = i
            while not visited[cur]:
                visited[cur] = True
                cycle.append(cur)
                cur = P0[cur]
            cycles.append(cycle)
    
    cycle_lengths = [len(cycle) for cycle in cycles]
    k = 1
    for L in cycle_lengths:
        k = k * L // gcd(k, L)
    
    list_of_permutations = []
    for exponent in range(1, k+1):
        arr = [0] * n
        for cycle in cycles:
            L_cycle = len(cycle)
            for j in range(L_cycle):
                current_index = cycle[j]
                next_index_in_cycle = (j + exponent) % L_cycle
                next_index = cycle[next_index_in_cycle]
                arr[current_index] = next_index
        list_of_permutations.append((tuple(arr), exponent))
    
    list_of_permutations.sort(key=lambda x: x[0])
    exponents_sorted = [exp for (perm, exp) in list_of_permutations]
    
    output_lines = []
    for K in queries:
        output_lines.append(str(exponents_sorted[K-1]))
    
    sys.stdout.write("\n".join(output_lines))

if __name__ == '__main__':
    main()