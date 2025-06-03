# Problem Statement
Pichuu has just entered the Safari Zone and is amazed by all the rare Pokemon in the Safari Zone. The Safari Zone can be divided into different zones labelled 1 to $N$ arranged in a line from left to right. Using the binoculars from the viewing platform, Pichuu was able to scout all the different zones and notice that there is exactly one pokemon in each zone of species $P_i$ (These Pokemon may be highly territorial).

Since, time in the Safari Zone is limited, Pichuu is planning different routes before entering the area. Each route consist of Pichuu taking the tram to Zone $A$ and walking right till Zone $B$ and then taking the tram back to the entrance. Due to Pichuu’s severe "Gotta Catch Em All" syndrome, he must and will catch any Pokemon on his route (He is a very good catcher), inclusive of the zone he starts and end in.

However, as the Pokemon in the Safari Zone are endangered, if Pichuu catches more than $K$ of same species of Pokemon after he finishes his route, the Pokemon Ranger will confiscate all of Pichuu’s Pokemon of that species. He wishes to catch as many different species of Pokemon as possible to add to his team. Thus, he needs to calculate the number of different species that can be obtained in each of his routes.

However, while planning routes, he may occasionally use the binoculars again to check a zone $A$, and realise that the Pokemon in a zone has changed to species $B$ (Maybe the original Pokemon lost the territory battle). Thus, all of his future plans need to take into account this change when calculating number of different species that can be obtained.

Pichuu is bad at counting and still has to use the Poketch Counter function to count, please help him instead!

# Constraints

- $1 \le N, M, K \le 10^5$
- $1 \le P_i, A_i, B_i \le N$
- $1 \le Q_i \le 2$
- If $Q_i = 2$, then $A_i \le B_i$

# Input

The first line contains three integers, $N, M, K$, which are the number of zones, total number of plans and changes in Pokemon, limit of number of Pokemon of the same species that can be caught without being confiscated.

The second line contains $N$ integers, $P_i$ which is the original species of the Pokemon in zone $i$.

$M$ lines then follow each with three integers: $Q_i, A_i, B_i$
- If $Q_i = 1$, then the Pokemon in zone $A_i$ changes to species $B_i$
- If $Q_i = 2$, then Pichuu is planning a route from zone $A_i$ to $B_i$

# Output

For each query with $Q_i = 2$, output a single integer representing the number of different species that can be obtained from the route.

# Sample Input #1
```
5 5 2
1 2 2 2 1 
2 1 4
1 3 3
2 1 4
1 3 3
2 1 5
```
# Sample Output #1
```
1
3
3
```
# Sample Input #2
```
10 10 3
5 2 5 2 5 2 5 2 3 2 
2 6 10
2 1 9
1 7 2
1 9 2
1 2 3
2 5 9
1 10 3
2 1 9
2 3 10
2 2 8
```
# Sample Output #2
```
3
1
1
2
2
2
```