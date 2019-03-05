#include "cache.h"
#include <stdio.h>
#include <time.h>

#define MAX_SIZE 1024

#define ARR_ROW_SIZE MAX_SIZE
#define ARR_COL_SIZE MAX_SIZE

typedef long array_t[ARR_ROW_SIZE * ARR_COL_SIZE];

static cache_t *cache;

/* Array used for testing. The aligned attribute is used to ensure
 * that test_array[0] is stored at the start of a cache block, for any
 * block size up to 1024.
 */
array_t __attribute__ ((aligned (MAX_SIZE))) test_array;

/*
 * Write data to the array, bypassing the "cache".
 */
void fillArray(array_t a) {
  
  int i, j;
  
  for (i = 0; i < ARR_ROW_SIZE; i++)
    for (j = 0; j < ARR_COL_SIZE; j++)
      a[i * ARR_COL_SIZE + j] = (i + 1) * 0x100 + (j + 1);
}

long sumA(array_t a, int rows, int cols) {
  
  int i, j;
  long sum = 0;

  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      sum += cache_read(cache, &a[i * cols + j]);
  
  return sum;
}

long sumB(array_t a, int rows, int cols) {
  
  int i, j;
  long sum = 0;

  for (j = 0; j < cols; j++)
    for (i = 0; i < rows; i++)
      sum += cache_read(cache, &a[i * cols + j]);
  
  return sum;
}

long sumC(array_t a, int rows, int cols) {
  
  int i, j;
  int sum = 0;
  
  for (j = 0; j < cols; j += 2)
    for (i = 0; i < rows; i += 2)
      sum += cache_read(cache, &a[i * cols + j]) +
	cache_read(cache, &a[(i+1) * cols + j]) +
	cache_read(cache, &a[i * cols + j+1]) +
	cache_read(cache, &a[(i+1) * cols + j+1]);
  
  return sum;
}

void print_stats() {
  
  int mc = cache_miss_count(cache);
  int ac = cache_access_count(cache);
  
  if (ac == 0) {
    printf("The cache wasn't used.\n");
  }
  else {
    printf("Miss rate = %8.4f\n", (double) mc/ac);
  }
}

int main() {

  srand(time(NULL));
  
  fillArray(test_array);
  
  cache = cache_new(256, 64, 1, CACHE_REPLACEMENTPOLICY_LRU);
  printf("Sum = %ld\n", sumA(test_array, 64, 64));
  print_stats();
  
  cache = cache_new(256, 64, 1, CACHE_REPLACEMENTPOLICY_LRU);
  printf("Sum = %ld\n", sumB(test_array, 64, 64));
  print_stats();
  
  cache = cache_new(256, 64, 1, CACHE_REPLACEMENTPOLICY_LRU);
  printf("Sum = %ld\n", sumC(test_array, 64, 64));
  print_stats();
  
  // Part 2
  /*
  cache = cache_new(32, 64, 1, CACHE_REPLACEMENTPOLICY_LRU);
  printf("Sum = %ld\n", sumB(test_array, ARR_ROW_SIZE, ARR_COL_SIZE));
  print_stats();
  */

  return 0;
}
