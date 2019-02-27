
Put your answers to non-coding questions in this file as described in
the assignment description.

All answers are to be in plain text ASCII.

Mask Log:

mask1
	- Make the columns loop (i.e. the i variable) the inner loop for every loop
	- Since C is a row-major language this will drastically increase spacial and temporal locality 

mask2
	- Although every set of nested for loops is a tiny bit different, some have only one thing different between the two (e.g. top left vs above loops, row is the same for both, but column is different)
	- Improve efficiency by making a new variable (col1 = i-1) and combining the two sets of for loops, instead adding a couple more lines of code in the loop.

idea for mask3
	- Make everything in one nested for loop. This should make the most use of locality
		- However, will need to use many conditionals and this may outweigh any efficiency gained from the increased locality