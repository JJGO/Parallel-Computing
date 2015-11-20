Jose Javier Gonzalez
University of Michigan
EECS 587

Homework 4

All the code used is in the respective files fsp.cpp.
There are three versions of the program:
	-BFS : regular implementation with load balancing
	-BFS_unbalanced : like BFS but ignoring all load balancing to compare the benefits of load balancing the problem

Since C++ primitives are used the compiler must be g++

For ease of use a makefile is provided.

	make 		: will just compile the cpp into a executable file
	make clean 	: cleans the  executable files
	make run 	: will compile (if necessary) and run the program for OMP_NUM_THREADS threads, storing the results in Results.txt
	make sub 	: will execute a series of qsub commands for flux which will produce a 6 executions of each p processor

NOTE: if load balancing measurements want to be done, then all lines with //DEBUG need to be uncommented

Also pbs scripts are provided for the different number of processors ( p1.sh, p4.sh, p16.sh, p36.sh)