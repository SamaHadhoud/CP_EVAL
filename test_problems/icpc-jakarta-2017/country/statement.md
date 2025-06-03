# Problem Statement

There are $N$ countries in this world, numbered from $1$ to $N$. Each country has a country name and a country code. Both of them can be represented as a string. No two countries share the same country name. For simplicity, in this problem we assume that a string consists only uppercase English alphabets (A–Z).

One of the widely recognised country code standard is published by ISO (ISO 3166). However, Xenia has noticed something strange about ISO 3166. The resulting code is no longer alphabetical! For example, "INDIA" has “IN” as its country code while "INDONESIA" is “ID”. "INDONESIA" comes after "INDIA" in alphabetical order, but “IN" comes after “ID”.

Unhappy about this indecent property of the ISO 3166 country codes, Xenia seeks for a more consistent coding. She develops her own standard, called XEN 3166. The rules of XEN 3166 are:

- For each country, its country code must be a subsequence of its country name.
- For each country, its first letter of the country code must be the same as its first letter of the country name.
- For each country, the length of its country code must be exactly $K$.
- If a country with a country name $S$ has a country code $S'$ and a country with a country name $T$ has a country code $T'$, then $S$ is lexicographically smaller than $T$ if and only if $S'$ is lexicographically smaller than $T'$.

A string $T = T_1T_2\cdots T_{|T|}$ is a subsequence of a string $S = S_1S_2\cdots S_{|S|}$ if there exists a sequence of integers $u_1, u_2, \cdots, u_{|T|}$ where $1 \le u_1 < u_2 < \cdots < u_{|T|} \le |S|$ and $S_{u_i} = T_i$ for all $1 \le i \le |T|$. For example, "ID", "IN", and "IND" are subsequences of "INDIA", but "IDN", "INN", "Z" are not subsequences of "INDIA".

String $S = S_1S_2\cdots S_{|S|}$ is lexicographically smaller than string $T = T_1T_2\cdots T_{|T|}$ if at least one of the following is true:

- $|S| < |T|$ and $S_i = T_i$ for all $1 \le i \le |S|$, or
- There exists an index $i$ where $S_i < T_i$ and $S_j = T_j$ for all $1 \le j < i$.

For example, suppose there are only two countries and $K = 2$. The name of the first country is "INDIA" and the name of the second country is "INDONESIA". Therefore, several possible country codes assignment are:

- "ID" for "INDIA" and "IN" for "INDONESIA
- "IA" for "INDIA" and "ID" for "INDONESIA"
- "II" for "INDIA" and "IO" for "INDONESIA"

Given $N$, $K$, and the list of country names. Help Xenia to assign the country codes for each country that follows the XEN 3166 rule, or state that it is impossible to do so.

# Input

The first line contains two integers: $N$ $K$ $(1 \le N \le 1000;\ 1 \le K \le 200)$ in a line denoting the number of countries and the length of country codes.
The next $N$ following lines, each contains a string which length between $1$ and $200,!000$ consisting of uppercase English alphabets (A–Z).
The string on the $i$-th line denotes the country name of the $i$-th country.
The sum of the length of all country names are guaranteed to be not more than $200,!000$.
It is also guaranteed that no two countries share the same country name.

# Output

- If it is possible for Xenia to assign the country codes for each country that follows the XEN 3166 rule, output "YES" (quotes only for clarity) on the first line.
The next $N$ following lines, each contains a string. The string on the $i$-th line denotes the country code of the $i$-th country.
If there are more than one possible country codes assignment, you may output any of them.

- If it is not possible for Xenia to assign the country codes for each country that follows the XEN 3166 rule, just output "NO" (quotes only for clarity) in a single line.

# Sample Input
```
2 2
INDIA
INDONESIA
```
# Output for Sample Input
```
YES
ID
IN
```
# Sample Input
```
3 2
IBAA
IAAA
IAAC
```
# Output for Sample Input
```
NO
```
# Explanation for the 1st sample case

The scenario in the first sample is the same as the scenario given in the problem description.