#!/bin/bash

# argument is 1 or 2. requires out_galaxy_mask0.pgm exists
make clean

if [ "$1" == "1" ]
then
  make timemask-mask1
  ./timemask-mask1 galaxy.pgm 10 out_galaxy_mask1.pgm
  cmp -b out_galaxy_mask1.pgm out_galaxy_mask0.pgm
elif [ "$1" == "2" ]
then
  make timemask-mask2
  ./timemask-mask2 galaxy.pgm 10 out_galaxy_mask2.pgm
  cmp -b out_galaxy_mask2.pgm out_galaxy_mask0.pgm
else
  make clean
fi
