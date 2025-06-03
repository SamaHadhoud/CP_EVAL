This is the easiest problem in this contest.

For each team, we can count the number of problems solved by the team. We loop from the first team to the last team while keeping the index of the winning team so far and updating the winning team only if the current team solved more problems than the current winning team.

At the end of the loop, we can print the index of the winning team.