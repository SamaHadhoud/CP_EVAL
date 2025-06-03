# Problem Statement

Andi is getting married! He and his partner plan to have $N$ children. To avoid any hassle in the future, Andi wants to decide all their children’s name in advance. Specifically, he wants each child to have a name which is lexicographically larger than any of his/her older siblings. Of course, his partner also agrees with this idea. String $A$ is lexicographically larger than string $B$ if and only if $B$ is a prefix of $A$ or there exists an index $i$ where $A_i > B_i$ and $A_j = B_j$ for all $j < i$. Note that a proper name can be as short as one character, but it **cannot** be an empty string.

Life is good for Andi, until one day he told his soon-to-be-grandmother-in-law (i.e. his partner’s grandma) about this marriage plan. After learning that Andi plans to have $N$ children with her granddaughter, she gave him $N$ names to be used. Moreover, the $i^{th}$ name can only be used for the $i^{th}$ child.

After going through a long debate with her grandma, Andi came into an agreement: The $i^{th}$ child’s name should be a subsequence of the $i^{th}$ name her grandma gave. A string $A$ is a subsequence of string $B$ if $A$ can be obtained by deleting zero or more characters from $B$ without changing the remaining characters’ order, e.g., $ABC$ is a subsequence of $DAEBCBCB$, but $EFG$ is not a subsequence of $FABEGC$.

Even though Andi dislikes the given list of names, he still wants to impress his partner by showing that he can satisfy both her grandma’s wish and his own desire (i.e. each child’s name is lexicographically larger than any of his/her older siblings). However, Andi wonders, what is the maximum possible total length of their children names?

For example, let $N = 3$, and the names given by his partner’s grandma are $(\texttt{KARIM}, \texttt{PARBUDI}, \texttt{CHANDRA})$.  
Here are several example set of names which satisfies Andi’s desire:

- $[\texttt{AR}, \texttt{BI}, \texttt{CRA}]$ with a total length of $2 + 2 + 3 = 7$.
- $[\texttt{ARI}, \texttt{BUDI}, \texttt{CHANDRA}]$ with a total length of $3 + 4 + 7 = 14$.
- $[\texttt{ARIM}, \texttt{ARUDI}, \texttt{CHANDRA}]$ with a total length of $4 + 5 + 7 = 16$.
- $[\texttt{AIM}, \texttt{ARBUDI}, \texttt{CHANDRA}]$ with a total length of $3 + 6 + 7 = 16$.
- $\cdots$

Among all sets of names which satisfy Andi’s desire in this example, the maximum total length is $16$. Note that there might be cases where valid set of names cannot be obtained. In such case, you should output $-1$. For example, let $N = 2$ and the names given by his partner’s grandma are $(\texttt{ZORO}, \texttt{ANDI})$. In this example, all subsequences of the $2^{nd}$ name are lexicographically smaller than all subsequences of the $1^{st}$ name, thus, no possible valid set of names can be obtained.

# Input

Input begins with a line containing an integer $N$ $(1 \le N \le 15)$ representing the number of children. The next $N$ lines, each contains a string $S_i$ $(1 \le |S_i| \le 15)$ representing the $i^{th}$ name given by Andi’s soon-to-be-grandmother-in-law. $S_i$ contains only uppercase alphabets $(S_{ij} \in \{A - Z\})$.

# Output

Output contains an integer in a line representing the maximum possible total length of their children names, or $-1$ if no possible valid set of names can be obtained.

# Sample Input #1
```
3
KARIM
PARBUDI
CHANDRA
```
# Sample Output #1
```
16
```
# Sample Input #2
```
2
ZORO
ANDI
```
# Sample Output #2
```
-1
```
# Sample Input #3
```
7
HAVE
FUN
IN
ICPC
JAKARTA
TWENTY
EIGHTEEN
```
# Sample Output #3
```
25
```
# Explanation for the sample input/output #3

One possible solution is $[\texttt{AVE}, \texttt{FUN}, \texttt{IN}, \texttt{IPC}, \texttt{JAKARTA}, \texttt{NTY}, \texttt{TEEN}]$  
with a total length of $3 + 3 + 2 + 3 + 7 + 3 + 4 = \boxed{25}$.
