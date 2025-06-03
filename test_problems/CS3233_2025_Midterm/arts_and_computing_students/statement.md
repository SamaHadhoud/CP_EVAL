# Problem Statement

To encourage interdisciplinary collaboration, the School of Computing has decided to hold a dinner event for students in School of Computing and the Faculty of Arts and Social Sciences. The event will be held in a large hall with a long table that can seat $2N$ students. Interestingly, the $2N$ seats are arranged on a single line, with $N$ pairs of seats. Seats belonging to the same pair are adjacent in the line. Each seat will either contain a student from the School of Computing, a student from the Faculty of Arts and Social Sciences, or be empty. Two students sitting next to each other within the same pair of seats are called "buddies". There are at most $N$ pairs of buddies by definition.

The Dean of the School of Computing suspects that School of Computing students will start Leetcoding on their laptops if they do not have a student from the Faculty of Arts and Social Sciences as a buddy. To prevent this, the Dean wants to rearrange the students such that each student from the School of Computing has a buddy from the Faculty of Arts and Social Sciences.

The Dean can perform the following operation: he can choose a student sitting next to an empty seat and move that student to the empty seat. These two seats need not belong to the same pair.

Given the initial arrangement of students, can you help the Dean determine if it is possible to rearrange the students such that each student from the School of Computing has a buddy from the Faculty of Arts and Social Sciences?

# Input

The first line of input contains an integer $N$ $(1 \leq N \leq 2 \times 10^5)$, the number of pairs of seats.

The second line of input contains a string of length $2N$ consisting of the characters `C`, `A`, and `.`. The $i$-th character is `C` if the student in the $i$-th seat is from the School of Computing, `A` if the student is from the Faculty of Arts and Social Sciences, and `.` if the seat is empty.

# Output

If it is not possible to rearrange the students such that each student from the School of Computing has a buddy from the Faculty of Arts and Social Sciences, output `NO`.

Otherwise, ouptut `YES` on the first line. Output a string of length $2N$ on the second line, where the $i$-th character is `C` if the student in the $i$-th seat is from the School of Computing, `A` if the student is from the Faculty of Arts and Social Sciences, and `.` if the seat is empty. This string should represent the final arrangement of students after the Dean has rearranged the students.

If there are multiple possible answers, you may output any of them.

# Sample Input #1
```
3
C.AAAC
```
# Sample Output #1
```
YES
CAA.AC
```
# Sample Input #2
```
4
AAA.CCAC
```
# Sample Output #2
```
NO
```
# Sample Input #3
```
4
CA.A.C.A
```
# Sample Output #3
```
YES
CA..ACA.
```