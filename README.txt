
Put your answers to non-coding questions in this file as described in
the assignment description.

All answers are to be in plain text ASCII.

Miss Rate Predictions:

a. 0.125. The cache has 32 sets, and each set has one line. Since each block has 64 bytes, each block can store 8 longs. 
		So reading each row requires 16 lines. Reading the first two rows gives 32 cold misses. After reading the first two 
		rows the cache is full. So for the last 6 rows, reading all elements in each line gives 16 capacity misses. So in total
		there are 16*8 = 128 misses. We read the array 8*128 = 1024 times, so the miss rate is 128/1024 = 0.125.

b. 1. The algorithm reads the array along each column from top to bottom. Reading the 0th row gives us 8 cold misses.
		When reading any element in k-th row, the set index of the block taken would be (k*16)%32. So the set index of lines 
		read would be 0, 16, 0, 16... So from column 1 to 7, reading each element would induce a conflict miss. The same process
		repeats for column 8 to 16, until 120 to 128. So every read induces a miss. So miss rate = 1.

c. 0.5. The algorithm reads two consecutive columns at once, then proceeds to the next row. Since the array is stored in row-
		major order, each time reading the second column would induce a cache hit. Due to set index being 0, 16, 0, 16..., reading
		the first 2 columns induce 8 cold misses, and for the rest of the columns, accessing two elements induces 1 cache miss. So
		the miss rate is 0.5.

d. 1. The cache has 32/2 = 16 sets, and each set has 2 lines. Reading the first column induces 8 cold 
		misses. Again the set index of each line from top row to bottom row is 0, 16, 0, 16, ..., so after reading the first
		column, the lines in set 0 are a[4][0] to a[4][7] and a[6][0] to a[6][7]; the lines in set 1 are a[5][0] to a[5][7] and
		a[7][0] to a[7][7]. So reading the first 4 rows of column 1 would induce 4 conflict misses. For the same reason, reading
		the next 4 rows would also induce 4 conflict misses. So all reads induce misses.

e. 1. The cache has 32/4 = 8 sets, For each 8 columns, all rows map to the same cache set. For example,	for column 0 to 7, 
		all blocks are mapped to set 0. Due to LRU being used, the first column in each 8 columns induces 8 cold misses; the 
		next 7 columns have every read inducing a conflict miss. So every read is a miss.

f. Worst case: 1. The cache has 32/4 = 8 sets, For each replacement the Random Policy produces the same decision as LRU. 
		So every read is a miss.
	 Best case: 0.78125.  Random Replacement Policy is used. For each 8 columns, reading the first column always induce 8 cold
	 	misses. In the best scenario in the last 7 columns, each column has 2 hits. So for each 8 columns we have (8 + 6*7) = 50
		misses. So for all 128 columns we have 16*50 misses. So miss rate = 16*50/1024 = 0.78125. 

g. 0.125. The algorithm starts from reading a[0][0] to a[7][0]. This induces 8 cold misses. Each read moves a block into a
		different cache set, so for the next 7 columns every read is a hit. Similarly for the rest of the columns, for each 8
		columns, each time reading the first column, every block is mapped to a different set. So for every 8 columns we have 8 misses.
		So in total we have 8*(128/8)=128 misses. So miss rate = 128/1024 = 0.125.

Mask Log:

Tested on lin12 server

mask number  measurement times (usec)    average time (usec)     performance
    0           1868607                      1861028                  1
								1858842
								1861626
								1864755
								1856049
								1856769
								1854747
								1862005
								1860334
								1866547

		1           269833                         270141               0.145
								270272
								268702
								269379
								269556
								269131
								268869
								272152
								273769
								269749

		2           114316												 114384               0.061
								114749
								114173
								114241
								114357
								114549
								114297
								114288
								114537
								114333

Changes:
mask1:
- Make the columns loop (i.e. the i variable) the inner loop for every loop.
  Since C is a row-major language this will drastically increase spacial locality

mask2:
- Combined new image initialization, counting and producing result in one nested for loop. 
  This should make the most use of temporal locality
- Got rid of the weight matrix since it is not required for post condition of the mask function. 
  Instead used weight_temp, a temporary long variable to hold weight for newImage[j][i] in each iteration. 
	This reduces the cache load, and has temporal locality
- Used temp variables to hold intermediate values of newImage[j][i]. This reduces dependency between 
  instructions, thus allowing CPUs to exploit more parallelism
- Used registers for all local variables to reduce memory access