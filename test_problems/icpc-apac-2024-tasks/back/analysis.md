We would like to compute the sum of the route that has the minimum total traversal time (the best route) and the route that has the second minimum total traversal time (the second best route). Finding the best route can be done with Dijkstra algorithm. We can also note the set of roads traversed in this route.

To find the second best route, we run Dijkstra algorithm twice to compute, for each city $u$, the shortest path to city $1$ (denoted as $S_u$) and the shortest path to city $n$ (denoted as $T_u$). We then iterate each road that is not traversed in either direction in the best route to try which edge is traversed in the second best route. For each such road $(u,v)$, the minimum total traversal time for the second best route by traversing this edge is  
$S_u + w(u,v) + T_v$  
where $w(u,v)$ is the time it takes to traverse the road. We then take the minimum value among all such roads.

This solution runs in $O(n \log n)$ time.