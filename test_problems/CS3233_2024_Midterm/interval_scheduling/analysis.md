*Let’s solve a simpler problem: Assume we already have a target duration, and we would like to extend the time intervals to (at least) match that target duration.*

This can be solved using a greedy algorithm: consider the intervals in chronological order. For each interval considered, set the starting time of the interval to be right after the ending time of the previous interval, and set the ending time of the interval so that it is “just long enough”. There are no possible solutions if this ending time overlaps with the next interval, or if the ending time of the final interval is being modified.

With the simpler problem solved, we can solve the original problem by binary (or linear) searching on the answer. This solution runs in $\mathcal{O}(N \log T)$ (or $\mathcal{O}(NT)$) time, where $T$ is the maximum possible duration.
