# Problem Statement

Andi is a young and prominent detective in the police force. His ability to track down criminals, uncover the truth, and solve cases never ceases to amaze all of his colleagues. One day, he is faced with a suspicious eyewitness testimony when working on a certain case. In usual cases, Andi simply ignores such unreliable testimony; however, in this case, the eyewitness testimony is too important to be ignored. To resolve this situation, Andi has to rely on technology, i.e. using a lie detector.

Andi proceeds to use a lie detector to detect whether the eyewitness testimony is true. However, Andi notices that the lie detector he used might have been tampered, thus, he employs a second lie detector to detect whether the first lie detector’s result is correct. This situation happens repeatedly such that Andi ends up employing $N$ lie detectors in total. The $i^{th}$ lie detector reports the truth of the $(i-1)^{th}$ lie detector for $i = 2..N$, and the $1^{st}$ lie detector reports the truth of the eyewitness testimony.

In the end, Andi knows that the last $(N^{th})$ lie detector has not been tampered and always report the truth correctly. Now, he needs to determine whether the eyewitness testimony is true given the result of all lie detectors.

For example, let $N = 4$ and the lie detectors result are (LIE, LIE, TRUTH, TRUTH).

- The $4^{th}$ lie detector reports that the $3^{rd}$ lie detector is TRUTH. As the $4^{th}$ lie detector always report the truth correctly, then the $3^{rd}$ lie detector’s result is correct as it is.
- The $3^{rd}$ lie detector reports that the $2^{nd}$ lie detector is TRUTH. As the $3^{rd}$ lie detector’s result is correct as it is, then the $2^{nd}$ lie detector’s result is also correct as it is.
- The $2^{nd}$ lie detector reports that the $1^{st}$ lie detector is LIE. As the $2^{nd}$ lie detector’s result is correct as it is, then the $1^{st}$ lie detector’s result is wrong.
- The $1^{st}$ lie detector reports that the eyewitness testimony is LIE. As the $1^{st}$ lie detector’s result is wrong, then the eyewitness testimony is correct; in other words, what the eyewitness says is true.

Therefore, the eyewitness testimony in this example is true.

# Input

Input begins with a line containing an integer $N$ $(2 \le N \le 100000)$. The next $N$ lines, each contains a string $S_i$ (either `TRUTH` or `LIE`) representing the output of the $i^{th}$ lie detector for $i = 1..N$ respectively.

# Output

Output contains a string `TRUTH` or `LIE` in a line whether the eyewitness testimony is true or false.

# Sample Input #1
```
4
LIE
LIE
TRUTH
TRUTH
```
# Sample Output #1
```
TRUTH
```
# Explanation for the sample input/output #1

This sample is illustrated in the problem description above.

# Sample Input #2
```
3
LIE
LIE
LIE
```
# Sample Output #2
```
LIE
```