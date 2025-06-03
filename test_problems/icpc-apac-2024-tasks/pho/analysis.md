Consider an easier version of this problem where we can add new tables. Clearly, it is optimal if, for the tables that are seated by different orders, we move the _minority_, i.e. the lesser orders between the two types of pho, to a new table.

One can assume that this strategy also works for the original problem. However, it only works if existing tables accommodate the minorities. This will not be an issue if we can have both types of pho to be a non-minority on some tables. But in case non-minorities are just one type (which also means there is just one type of minority), we need to choose a table that is dedicated to accommodating minorities instead. As this table will move the non-minority orders, it is not optimal to choose more than one dedicated table. Thus, a single careful iteration will suffice.

There is also an alternative solution that uses DP-Bitmask where we have four bits to represent: there is a table where we need to move ‘0’, there is a table where we need to move ‘1’, there is a table dedicated to consist of only ‘0’, and there is a table dedicated to consist of only ‘1’.

All solutions run in $O(n + \sum_{i=1}^{n} |S_i|)$.