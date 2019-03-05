#include <stdio.h>
#include <stdlib.h>
#include "mask.h"

// When completed this file will contain several versions of the
// mask() function and conditional compilation will be used to
// determine which version runs.  Each version will be named mask1,
// mask2, and so on. To select the version of the mask function to
// use, change the following declaration to the version of the mask()
// to be used.  All versions of mask() must produce a correct result.

// The provided Makefile also includes the possibility of creating an
// executable called "timemask-maskN" (replacing maskN with any
// version you'd like to test). You may compile it with "make
// timemask-maskN" then run "./timemask-maskN".

#ifndef MASK_VERSION
#define MASK_VERSION mask0
#endif

// You are only allowed to change the contents of this file with
// respect to improving the performance of this program. You may not
// change the data structure for each image or the parameters for the
// function mask. You may, however, change the structure of the code
// and the types/implementation of local variables (col, row, weight,
// etc.).


// You are required to document the changes you make in the README.txt
// file. For each entry in the README.txt file there is to be a
// version of the matching mask() function here such that the markers
// can see, and run if needed, the actual code that you used to
// produce your results for the logged change to the mask function.

static inline long mask0(long oldImage[N][N], long newImage[N][N], int rows, int cols) {
  return baseMask(oldImage, newImage, rows, cols);
}

// For each version of mask you are testing duplicate the function
// below, rename it to a different name, and make your changes. To use
// the new version, change the #define above to use the current version.

static inline long mask1(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask0 by
  // making better use of caching.
    
  int i, j;
  int col, row;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));
  
  //initialize the new image
  for (j = 0; j < rows; j++)
    for (i = 0; i < cols; i++) {
      newImage[j][i] = WEIGHT_CENTRE * oldImage[j][i];
      weight[j][i] = WEIGHT_CENTRE;
    }
  
  // Count the cells to the top left
  for (j = 1; j < rows; j++) {
    row = j - 1;
    for (i = 1; i < cols; i++) {
      col = i - 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }
  
  // Count the cells immediately above
  for (j = 1; j < rows; j++) {
    row = j - 1;
    for (i = 0; i < cols; i++){
      newImage[j][i] += WEIGHT_SIDE * oldImage[row][i];
      weight[j][i] += WEIGHT_SIDE;
    }
  }
  
  // Count the cells to the top right
  for (j = 1; j < rows; j++) {
    row = j - 1;
    for (i = 0; i < cols - 1; i++) {
      col = i + 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }
  
  // Count the cells to the immediate left
  for (j = 0; j < rows; j++) {
    for (i = 1; i < cols; i++){
      col = i - 1;
      newImage[j][i] += WEIGHT_SIDE * oldImage[j][col];
      weight[j][i] += WEIGHT_SIDE;
    }
  }
  
  // Count the cells to the immediate right
  for (j = 0; j < rows; j++) {
    for (i = 0; i < cols - 1; i++) {
      col = i + 1;
      newImage[j][i] += WEIGHT_SIDE * oldImage[j][col];
      weight[j][i] += WEIGHT_SIDE;
    }
  }
  
  // Count the cells to the bottom left
  for (j = 0; j < rows - 1; j++) {
    row = j + 1;
     for (i = 1; i < cols; i++) {
      col = i - 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }
  
  // Count the cells immediately below
  for (j = 0; j < rows - 1; j++) {
    row = j + 1;
    for (i = 0; i < cols; i++) {
      newImage[j][i] += WEIGHT_SIDE * oldImage[row][i];
      weight[j][i] += WEIGHT_SIDE;
    }
  }
  
  // Count the cells to the bottom right
  for (j = 0; j < rows - 1; j++) {
    row = j + 1;
    for (i = 0; i < cols - 1; i++) {
      col = i + 1;
      newImage[j][i] += WEIGHT_CORNER * oldImage[row][col];
      weight[j][i] += WEIGHT_CORNER;
    }
  }

  // Produce the final result
  
    for (j = 0; j < rows; j++)
      for (i = 0; i < cols; i++) {
      newImage[j][i] = newImage[j][i] / weight[j][i];
      check += newImage[j][i];
    }
  
  return check;
}

static inline long mask2(long oldImage[N][N], long newImage[N][N], int rows, int cols) {

  // TODO This function should contain code that produces the same
  // output as baseMask, but is expected to run faster than mask1 (or
  // mask0) by making better use of caching.
  int i, j;
  int col_topleft, col_topright, col_immleft, col_immright, col_botleft, col_botright; 
  int row_topleft, row_immabove, row_topright, row_botleft, row_immbelow, row_botright;
  long check = 0;

  long (*weight)[N] = calloc(N * N, sizeof(long));
    
  // initialize the new image, count the cells, produce results
  for (j = 0; j < rows; j++) {
    row_topleft = j - 1;
    row_immabove = j - 1;
    row_topright = j - 1;
    row_botleft = j + 1;
    row_immbelow = j + 1;
    row_botright = j + 1;

    for (i = 0; i < cols; i++) {

      // initialize new image
      newImage[j][i] = WEIGHT_CENTRE * oldImage[j][i];
      weight[j][i] = WEIGHT_CENTRE;

      // top left
      if(i>=1 && j>=1){
        col_topleft = i - 1;
        newImage[j][i] += WEIGHT_CORNER * oldImage[row_topleft][col_topleft];
        weight[j][i] += WEIGHT_CORNER;
      }

      // immediately above
      if(j>=1 && i>=0){
        newImage[j][i] += WEIGHT_SIDE * oldImage[row_immabove][i];
        weight[j][i] += WEIGHT_SIDE;
      }

      // immediate left
      if(j>=0 && i>=1){
        col_immleft = i - 1;
        newImage[j][i] += WEIGHT_SIDE * oldImage[j][col_immleft];
        weight[j][i] += WEIGHT_SIDE;
      }

      // top right
      if(j>=1 && i>=0 && i<cols-1){
        col_topright = i + 1;
        newImage[j][i] += WEIGHT_CORNER * oldImage[row_topright][col_topright];
        weight[j][i] += WEIGHT_CORNER;
      }

      // immediate right
      if(i<cols-1){
        col_immright = i + 1;
        newImage[j][i] += WEIGHT_SIDE * oldImage[j][col_immright];
        weight[j][i] += WEIGHT_SIDE;
      }

      // bottom left
      if(j<rows-1 && i>=1){
        col_botleft = i - 1;
        newImage[j][i] += WEIGHT_CORNER * oldImage[row_botleft][col_botleft];
        weight[j][i] += WEIGHT_CORNER;
      }

      // immediate below
      if(j<rows-1){
        newImage[j][i] += WEIGHT_SIDE * oldImage[row_immbelow][i];
        weight[j][i] += WEIGHT_SIDE;
      }

      // bottom right
      if(j<rows-1 && i<cols-1){
        col_botright = i + 1;
        newImage[j][i] += WEIGHT_CORNER * oldImage[row_botright][col_botright];
        weight[j][i] += WEIGHT_CORNER;
      }

      // produce results
      newImage[j][i] = newImage[j][i] / weight[j][i];
      check += newImage[j][i];
    }
  }

  return check;
}

long mask(long oldImage[N][N], long newImage[N][N], int rows, int cols) {
  return MASK_VERSION(oldImage, newImage, rows, cols);
}

