The problem description makes the problem looks harder than it should be. Once you understand the problem better, you will realise that it actually is a fairly easy problem.

The first three requirements simply denote a forest with rooted trees where each vertex and edge has a label/value (not necessarily unique). Now, let's observe the following four points:

- (a) There are $N$ tuples $(u_i, v_i, w_i)$,
- (b) Each tuple appears exactly once as an edge in the forest,
- (c) No two tuples have the same $w_i$,
- (d) The forest should contain exactly $N$ edges.

These points imply that there is a one-to-one relationship between each tuple and edge in the forest.  
So, processing a tuple is equal to adding an edge (and vertex) in the forest.

Requirement #5 forces us to prioritise tuples with smaller $w_i$ to be closer to the root than tuples with larger $w_i$.  
We can construct the same forest by processing each tuple one by one in increasing order of $w_i$.  
When we process a tuple $(u_i, v_i)$, add a new vertex with label $v_i$ as a child of an existing vertex with label $u_i$ which still has fewer than $M$ children, or create a new vertex with label $u_i$ as a root of a new tree and add vertex with label $v_i$ as its child.

The key observation is: It doesn't matter which vertex with label $u_i$ we choose when we process the tuple $(u_i, v_i)$, as long as the chosen vertex still has fewer than $M$ children; otherwise, a new tree is inevitable (greedy solution).

**Solution**:  
Sort all the tuples $(u_i, v_i, w_i)$ based on $w_i$ in ascending order. Add each tuple in this order into the forest one at a time, i.e. add a new vertex with label $v_i$ as a child of *any* existing vertex with label $u_i$ which still has fewer than $M$ children; if there is no such vertex, then create a new tree with label $u_i$ as its root, and put vertex with label $v_i$ as its child.

Note that when implementing the solution, we don't need to actually build the forest.  
We only need to keep track the number of trees, the number of nodes for each label, and the number of existing children for each label. Alternatively, the last two can be combined into the number of possible new children for each label.

Here is one implementation in C/C++. The time-complexity for this solution is $O(N \log N)$.
```
#include <bits/stdc++.h>
using namespace std;

struct tuple { int u, v, w; };
bool operator < (const tuple &x, const tuple &y) { return x.w < y.w; };

int N, M;
tuple arr[100005];
map <int,int> cap;
int   main()     {
    scanf( "%d %d", &N, &M ); 
    for ( int i = 0; i < N; i++ )
        scanf( "%d %d %d", &arr[i].u, &arr[i].v, &arr[i].w );
    sort(arr,arr+N);
    int ans = 0;
    for ( int i = 0; i < N; i++ ) {
        if ( cap[arr[i].u] == 0 ) {
        cap[arr[i].u] += M - 1;
        cap[arr[i].v] += M;
        ans++;
        }
        else {
            cap[arr[i].u]--;
            cap[arr[i].v] += M;
        }
    }
    printf( "%d\n", ans );
    return 0;
}
```