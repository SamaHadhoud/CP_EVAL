# Problem Statement

*Romanos: "Can I submit an interactive problem to ACM-ICPC 2017 Jakarta?"  
Theodora: "No."  
Romanos: "Aww, man.. That's not fun."*

Then Romanos decided to submit a non-interactive version of his problem to the contest; and here it is, based on his playing with Theodora.

Romanos and Theodora are playing a game. Initially, Theodora picks a number between $1$ to $N$ inclusive. Romanos' goal is to determine that number. He can make up to $K$ guesses. For each guess, he will say a number out loud as his guess. Theodora will then say one of the following answers based on the exact condition:
- "My number is smaller than your guess ($<$)",
- "My number is greater than your guess ($>$)", or
- "Your guess is correct ($=$)".

The game will end right after one of the following:
- Romanos guesses the correct number (he wins), or
- All $K$ guesses are wrong (he loses).

Sadly, Romanos is not playing the game seriously. He knows the best strategy to win this game, but he does not always use it. Nevertheless, he pretends that he plays seriously, hence his guess will never be a dumb one. In other words, his guess will always be a number between $1$ to $N$ inclusive and will always be consistent with all of Theodora's previous answers.

For example, suppose $N$ is $10$ and Romanos' first guess is $4$. If Theodora answers "My number is smaller than your guess ($<$)" then the next Romanos's guess will always be between $1$ to $3$, inclusive.

Contrast to Romanos, Theodora is playing the game too seriously. She wants to win (by making Romanos lose) and "cheats" as follows.

Adaptively, Theodora might change her number as far as it is consistent with all of her previous answers. To do that, whenever possible, Theodora will always answer either "My number is smaller than your guess ($<$)" or "My number is greater than your guess ($>$)" that maximizes the possible answer range. In the case of tie, she will always answer the first one ($<$).

For example, suppose $N = 10$ and Romanos' first guess is $4$. Theodora will always answer "My number is greater than your guess ($>$)" because the possible answer range will be between $5$ to $10$ inclusive; it is larger than $1$ to $3$ inclusive.

Now, this is the actual problem proposed by Romanos. You are given $N$, $K$, and Theodora's answer for each guess. Can you show one of the possible scenario for Romanos' guesses or state that it is impossible?

# Input

The first line contains two integers: $N$ $K$ ($1 \leq N \leq 10^{18};\ 1 \leq K \leq 50{,}000$) in a line denoting the number range and the number of guesses.  
The second line contains a string in a line denoting Theodora's answers.  
Each character of the string is either `<`, `>`, or `=`, and either:
- The last character of the string is `=` and each character of the string other than the last character is either `<` or `>`, and the length of the string is not more than $K$, **or**
- The length of the string is exactly $K$ and each character of the string is either `<` or `>`.

# Output

- If there is a possible scenario for Romanos' guesses, the output contains $M$ integers: $A_1\ A_2\ \cdots\ A_M$ in a line denoting Romanos' guesses where $M$ is the length of the string and $A_i$ is Romanos' $i$-th guess. If there are more than one possible scenario for Romanos' guesses, you may output any of them.
- If there is no possible scenario for Romanos' guesses, just output `-1` in a line.

# Sample Input
```
10 5
><>=
```
# Output for Sample Input
```
5 8 6 7
```
# Sample Input
```
10 5
><<><
```
# Output for Sample Input
```
4 10 8 5 7
```
# Sample Input
```
10 5
<>=
```
# Output for Sample Input
```
-1
```
# Sample Input
```
10 9
>>>>>>>>
```
# Output for Sample Input
```
1 2 3 4 5 6 7 8 9
```
# Sample Input
```
10 10
>>>>>>>>>>
```
# Output for Sample Input
```
-1
```