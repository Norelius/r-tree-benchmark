#!/bin/bash

./rstar.out time 100 2 0 > rstar100.dat
./rstar.out time 100 2 1 > rstar100constant.dat
./rstar.out build 100 2 0 > rstar100_build.dat
./rstar.out build 100 2 1 > rstar100constant_build.dat
./rstar.out time 100 1 0 > quadratic100.dat
./rstar.out time 100 1 1 > quadratic100constant.dat
./rstar.out build 100 1 0 > quadratic100_build.dat
./rstar.out build 100 1 1 > quadratic100constant_build.dat
# gnuplot *.gnu