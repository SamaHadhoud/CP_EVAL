#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const int MAXN = 100005;

struct Query {
    int time, l, r, idx;
};

struct Update {
    int pos, old_val, new_val;
};

int block_size;
vector<int> arr;
vector<Query> queries;
vector<Update> updates;
vector<int> ans;
vector<int> freq;
int distinct_count, bad_count;
int N, M, K;

bool cmp(const Query &a, const Query &b) {
    int block_a_l = a.l / block_size;
    int block_b_l = b.l / block_size;
    if (block_a_l != block_b_l) {
        return block_a_l < block_b_l;
    }
    int block_a_r = a.r / block_size;
    int block_b_r = b.r / block_size;
    if (block_a_r != block_b_r) {
        if (block_a_l % 2 == 0) {
            return block_a_r < block_b_r;
        } else {
            return block_a_r > block_b_r;
        }
    }
    return a.time < b.time;
}

void add_element(int i) {
    int species = arr[i];
    int &count = freq[species];
    if (count == 0) {
        distinct_count++;
    }
    count++;
    if (count == K + 1) {
        bad_count++;
    }
}

void remove_element(int i) {
    int species = arr[i];
    int &count = freq[species];
    if (count == K + 1) {
        bad_count--;
    }
    count--;
    if (count == 0) {
        distinct_count--;
    }
}

void apply_update(const Update &u, int current_l, int current_r) {
    if (u.old_val == u.new_val) {
        return;
    }
    if (current_l <= u.pos && u.pos <= current_r) {
        remove_element(u.pos);
        arr[u.pos] = u.new_val;
        add_element(u.pos);
    } else {
        arr[u.pos] = u.new_val;
    }
}

void unapply_update(const Update &u, int current_l, int current_r) {
    if (u.old_val == u.new_val) {
        return;
    }
    if (current_l <= u.pos && u.pos <= current_r) {
        remove_element(u.pos);
        arr[u.pos] = u.old_val;
        add_element(u.pos);
    } else {
        arr[u.pos] = u.old_val;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> K;
    arr.resize(N);
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
        arr[i]--;
    }

    vector<int> current_array = arr;
    int time_count = 0;
    int query_count = 0;
    vector<int> output_ans;

    for (int i = 0; i < M; i++) {
        int q, a, b;
        cin >> q >> a >> b;
        if (q == 1) {
            a--;
            int b0 = b - 1;
            int old_val = current_array[a];
            if (old_val != b0) {
                current_array[a] = b0;
            }
            updates.push_back({a, old_val, b0});
            time_count++;
        } else {
            a--;
            b--;
            queries.push_back({time_count, a, b, query_count++});
        }
    }

    if (queries.empty()) {
        return 0;
    }

    block_size = (int)pow(N, 2.0/3.0);
    if (block_size < 1) block_size = 1;
    sort(queries.begin(), queries.end(), cmp);

    arr = current_array;
    freq.assign(MAXN, 0);
    distinct_count = 0;
    bad_count = 0;
    int current_l = 0, current_r = -1, current_time = updates.size();
    ans.resize(query_count);

    for (const auto &q : queries) {
        while (current_time < q.time) {
            apply_update(updates[current_time], current_l, current_r);
            current_time++;
        }
        while (current_time > q.time) {
            current_time--;
            unapply_update(updates[current_time], current_l, current_r);
        }

        while (current_l > q.l) {
            add_element(--current_l);
        }
        while (current_r < q.r) {
            add_element(++current_r);
        }
        while (current_l < q.l) {
            remove_element(current_l++);
        }
        while (current_r > q.r) {
            remove_element(current_r--);
        }

        ans[q.idx] = distinct_count - bad_count;
    }

    for (int i = 0; i < query_count; i++) {
        cout << ans[i] << '\n';
    }

    return 0;
}