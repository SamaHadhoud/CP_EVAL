import heapq
import itertools

def main():
    import sys
    data = sys.stdin.read().strip().split()
    if not data:
        return
    
    it = iter(data)
    N = int(next(it)); M = int(next(it))
    R = []
    for _ in range(3):
        row = [int(next(it)) for _ in range(3)]
        R.append(row)
    
    def read_config(it):
        config = []
        for _ in range(3):
            k = int(next(it))
            bricks = []
            if k > 0:
                bricks = [int(next(it)) for _ in range(k)]
            config.append(frozenset(bricks))
        return tuple(config)
    
    initial_config = read_config(it)
    goals = []
    for _ in range(M):
        config = read_config(it)
        goals.append(config)
    
    final0 = (frozenset(range(1, N+1)), frozenset(), frozenset())
    final1 = (frozenset(), frozenset(range(1, N+1)), frozenset())
    final2 = (frozenset(), frozenset(), frozenset(range(1, N+1)))
    finals = [final0, final1, final2]
    
    states_of_interest = [initial_config]
    for g in goals:
        if g not in states_of_interest:
            states_of_interest.append(g)
    for f in finals:
        if f not in states_of_interest:
            states_of_interest.append(f)
    
    n_interest = len(states_of_interest)
    cost_matrix = [[10**18] * n_interest for _ in range(n_interest)]
    
    if N > 10:
        print(0)
        return

    for idx, source in enumerate(states_of_interest):
        dist = {}
        pq = []
        heapq.heappush(pq, (0, source))
        dist[source] = 0
        found = [False] * n_interest
        count_found = 0
        
        while pq and count_found < n_interest:
            cost, state = heapq.heappop(pq)
            if cost != dist.get(state, 10**18):
                continue
            if state in states_of_interest:
                j = states_of_interest.index(state)
                if not found[j]:
                    found[j] = True
                    count_found += 1
                    if count_found == n_interest:
                        break
            
            parks = list(state)
            for from_park in range(3):
                if not parks[from_park]:
                    continue
                top = min(parks[from_park])
                for to_park in range(3):
                    if from_park == to_park:
                        continue
                    if parks[to_park]:
                        if top > min(parks[to_park]):
                            continue
                    new_from = parks[from_park] - {top}
                    new_to = parks[to_park] | {top}
                    new_parks = list(parks)
                    new_parks[from_park] = new_from
                    new_parks[to_park] = new_to
                    new_state = tuple(new_parks)
                    move_cost = R[from_park][to_park]
                    new_cost = cost + move_cost
                    if new_state not in dist or new_cost < dist[new_state]:
                        dist[new_state] = new_cost
                        heapq.heappush(pq, (new_cost, new_state))
        
        for j, target in enumerate(states_of_interest):
            if target in dist:
                cost_matrix[idx][j] = dist[target]
    
    INF = 10**18
    n_goals = M
    dp = [[INF] * n_interest for _ in range(1<<n_goals)]
    dp[0][0] = 0
    
    for mask in range(1<<n_goals):
        for i in range(n_interest):
            if dp[mask][i] == INF:
                continue
            for j in range(n_interest):
                if i == j:
                    continue
                if j < 1 or j > n_goals:
                    continue
                goal_index = j - 1
                if mask & (1<<goal_index):
                    continue
                new_mask = mask | (1<<goal_index)
                new_cost = dp[mask][i] + cost_matrix[i][j]
                if new_cost < dp[new_mask][j]:
                    dp[new_mask][j] = new_cost
    
    full_mask = (1<<n_goals) - 1
    final_indices = []
    for idx, state in enumerate(states_of_interest):
        if state in (final0, final1, final2):
            final_indices.append(idx)
    
    ans = INF
    for i in range(n_interest):
        if dp[full_mask][i] == INF:
            continue
        for f_idx in final_indices:
            total_cost = dp[full_mask][i] + cost_matrix[i][f_idx]
            if total_cost < ans:
                ans = total_cost
    print(ans)

if __name__ == "__main__":
    main()