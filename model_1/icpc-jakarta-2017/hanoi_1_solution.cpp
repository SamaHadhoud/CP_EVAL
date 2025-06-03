#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <climits>
#include <tuple>
#include <functional>
#include <cctype>
#include <cmath>
#include <set>

using namespace std;

typedef unsigned long long ULL;
typedef tuple<ULL, ULL, ULL> State;

const ULL INF = 1e18;

struct StateHash {
    size_t operator()(const State& s) const {
        auto hasher = hash<ULL>();
        size_t seed = hasher(get<0>(s));
        seed ^= hasher(get<1>(s)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(get<2>(s)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct StateEqual {
    bool operator()(const State& a, const State& b) const {
        return get<0>(a) == get<0>(b) && get<1>(a) == get<1>(b) && get<2>(a) == get<2>(b);
    }
};

vector<State> states_of_interest;
vector<vector<ULL>> dist_matrix;
ULL R[3][3];
int N, M;

vector<State> goal_states;
State finals[3];

State input_state() {
    ULL m0 = 0, m1 = 0, m2 = 0;
    for (int park = 0; park < 3; park++) {
        int k;
        cin >> k;
        for (int i = 0; i < k; i++) {
            int brick;
            cin >> brick;
            brick--; 
            if (park == 0) m0 |= (1ULL << brick);
            else if (park == 1) m1 |= (1ULL << brick);
            else if (park == 2) m2 |= (1ULL << brick);
        }
    }
    return State(m0, m1, m2);
}

vector<State> compute_dijkstra(const State& start, const vector<State>& targets) {
    unordered_map<State, ULL, StateHash, StateEqual> dist;
    priority_queue<pair<ULL, State>, vector<pair<ULL, State>>, greater<pair<ULL, State>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    set<State> targets_set(targets.begin(), targets.end());
    unordered_map<State, ULL, StateHash, StateEqual> found;

    while (!pq.empty()) {
        auto [cost, state] = pq.top();
        pq.pop();
        if (dist.find(state) != dist.end() && dist[state] < cost) 
            continue;

        if (targets_set.find(state) != targets_set.end()) {
            found[state] = cost;
            if (found.size() == targets_set.size()) 
                break;
        }

        ULL m0 = get<0>(state);
        ULL m1 = get<1>(state);
        ULL m2 = get<2>(state);
        ULL masks[3] = {m0, m1, m2};

        for (int from = 0; from < 3; from++) {
            if (masks[from] == 0) continue;
            int x = __builtin_ctzll(masks[from]);
            for (int to = 0; to < 3; to++) {
                if (from == to) continue;
                if (masks[to] == 0) {
                    ULL new_masks[3] = {m0, m1, m2};
                    new_masks[from] ^= (1ULL << x);
                    new_masks[to] |= (1ULL << x);
                    State new_state = make_tuple(new_masks[0], new_masks[1], new_masks[2]);
                    ULL new_cost = cost + R[from][to];
                    if (dist.find(new_state) == dist.end() || new_cost < dist[new_state]) {
                        dist[new_state] = new_cost;
                        pq.push({new_cost, new_state});
                    }
                } else {
                    int y = __builtin_ctzll(masks[to]);
                    if (x < y) {
                        ULL new_masks[3] = {m0, m1, m2};
                        new_masks[from] ^= (1ULL << x);
                        new_masks[to] |= (1ULL << x);
                        State new_state = make_tuple(new_masks[0], new_masks[1], new_masks[2]);
                        ULL new_cost = cost + R[from][to];
                        if (dist.find(new_state) == dist.end() || new_cost < dist[new_state]) {
                            dist[new_state] = new_cost;
                            pq.push({new_cost, new_state});
                        }
                    }
                }
            }
        }
    }

    vector<State> results;
    for (const State& target : targets) {
        if (found.find(target) != found.end()) {
            results.push_back(target);
        }
    }
    return results;
}

int main() {
    cin >> N >> M;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> R[i][j];
        }
    }

    State initial = input_state();
    states_of_interest.push_back(initial);

    goal_states.resize(M);
    for (int i = 0; i < M; i++) {
        goal_states[i] = input_state();
        states_of_interest.push_back(goal_states[i]);
    }

    finals[0] = State((1ULL << N) - 1, 0, 0);
    finals[1] = State(0, (1ULL << N) - 1, 0);
    finals[2] = State(0, 0, (1ULL << N) - 1);
    for (int i = 0; i < 3; i++) {
        states_of_interest.push_back(finals[i]);
    }

    int num_states = states_of_interest.size();
    dist_matrix.assign(num_states, vector<ULL>(num_states, INF));

    for (int i = 0; i < num_states; i++) {
        unordered_map<State, ULL, StateHash, StateEqual> dist;
        priority_queue<pair<ULL, State>, vector<pair<ULL, State>>, greater<pair<ULL, State>>> pq;

        dist[states_of_interest[i]] = 0;
        pq.push({0, states_of_interest[i]});

        set<State> found_set;
        unordered_map<State, ULL, StateHash, StateEqual> found_costs;

        while (!pq.empty()) {
            auto [cost, state] = pq.top();
            pq.pop();
            if (dist[state] < cost) 
                continue;

            for (int j = 0; j < num_states; j++) {
                if (state == states_of_interest[j]) {
                    found_set.insert(state);
                    found_costs[state] = cost;
                    if (found_set.size() == num_states) 
                        break;
                }
            }
            if (found_set.size() == num_states) 
                break;

            ULL m0 = get<0>(state);
            ULL m1 = get<1>(state);
            ULL m2 = get<2>(state);
            ULL masks[3] = {m0, m1, m2};

            for (int from = 0; from < 3; from++) {
                if (masks[from] == 0) continue;
                int x = __builtin_ctzll(masks[from]);
                for (int to = 0; to < 3; to++) {
                    if (from == to) continue;
                    if (masks[to] == 0) {
                        ULL new_masks[3] = {m0, m1, m2};
                        new_masks[from] ^= (1ULL << x);
                        new_masks[to] |= (1ULL << x);
                        State new_state = make_tuple(new_masks[0], new_masks[1], new_masks[2]);
                        ULL new_cost = cost + R[from][to];
                        if (dist.find(new_state) == dist.end() || new_cost < dist[new_state]) {
                            dist[new_state] = new_cost;
                            pq.push({new_cost, new_state});
                        }
                    } else {
                        int y = __builtin_ctzll(masks[to]);
                        if (x < y) {
                            ULL new_masks[3] = {m0, m1, m2};
                            new_masks[from] ^= (1ULL << x);
                            new_masks[to] |= (1ULL << x);
                            State new_state = make_tuple(new_masks[0], new_masks[1], new_masks[2]);
                            ULL new_cost = cost + R[from][to];
                            if (dist.find(new_state) == dist.end() || new_cost < dist[new_state]) {
                                dist[new_state] = new_cost;
                                pq.push({new_cost, new_state});
                            }
                        }
                    }
                }
            }
        }

        for (int j = 0; j < num_states; j++) {
            if (dist.find(states_of_interest[j]) != dist.end()) {
                dist_matrix[i][j] = dist[states_of_interest[j]];
            }
        }
    }

    if (M == 0) {
        ULL ans = INF;
        for (int f_index = num_states - 3; f_index < num_states; f_index++) {
            ans = min(ans, dist_matrix[0][f_index]);
        }
        cout << ans << endl;
        return 0;
    }

    vector<vector<ULL>> dp(1 << M, vector<ULL>(num_states, INF));
    dp[0][0] = 0;

    for (int mask = 0; mask < (1 << M); mask++) {
        for (int j = 0; j < num_states; j++) {
            if (dp[mask][j] == INF) continue;
            for (int k = 0; k < M; k++) {
                if (mask & (1 << k)) continue;
                int next_index = 1 + k;
                int new_mask = mask | (1 << k);
                ULL new_cost = dp[mask][j] + dist_matrix[j][next_index];
                if (new_cost < dp[new_mask][next_index]) {
                    dp[new_mask][next_index] = new_cost;
                }
            }
        }
    }

    ULL ans = INF;
    for (int j = 0; j < num_states; j++) {
        if (dp[(1<<M)-1][j] == INF) continue;
        for (int f_index = num_states - 3; f_index < num_states; f_index++) {
            if (dist_matrix[j][f_index] == INF) continue;
            ans = min(ans, dp[(1<<M)-1][j] + dist_matrix[j][f_index]);
        }
    }

    cout << ans << endl;

    return 0;
}