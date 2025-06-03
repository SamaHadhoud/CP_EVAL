# Problem Statement
Many couples will dine out on Valentine’s day. To serve as many customers as possible, a particular Western restaurant adopts the "Staggered Seating" strategy on the night of Valentine’s day.

The strategy works as follows: The restaurant divides its opening hours into $N$ non-overlapping time intervals, called a *wave*. In each wave, a group of customers are served and they must complete their meal (and leave the restaurant) within that time interval. Two intervals are said to be non-overlapping if the ending time of the first interval is not later than the starting time of the second interval (in chronological order).

The manager drafted the schedule of each wave. However, you discovered that some intervals are extremely short - customers are unable to enjoy the meal given the limited time.

You decided to come up with a better schedule, with the following conditions:

1. The amended time interval for each wave must be inclusive of the original time interval of that wave. Informally speaking, you are only allowed to extend the time intervals. Customers will feel unhappy if the time intervals are shortened.

2. The opening and closing time of the restaurant remains unchanged. That is, you cannot change the starting time of the first interval or the ending time of the last interval.

3. You would like to maximize the length of the shortest time interval across all waves.

Can you help the manager to come up with a better schedule?

# Input

The first line of input consists of a single positive integer $N$ $(2 \le N \le 200)$.

$N$ lines follow. Each line consists of a time interval in the format **HH:MM - HH:MM**.  
It is guaranteed that the time intervals are non-overlapping and given in chronological order.

It is also guaranteed that the time intervals are all within the same day, between **16:00** and **23:59** (inclusive).

# Output

Output $N$ lines, the amended schedule of the staggered seating plan. The format should be exactly same as that in the input.

If there are multiple solutions, you may output any of them.

# Sample Input #1
```
3
19:00 - 20:00
21:08 - 21:24
22:00 - 22:46
```
# Sample Output #1
```
19:00 - 20:15
20:15 - 21:30
21:31 - 22:46
```
# Sample Input #2
```
2
19:01 - 19:02
19:03 - 19:04
```
# Sample Output #2
```
19:01 - 19:03
19:03 - 19:04
```