Jose Javier Gonzalez
University of Michigan
EECS 587

Homework 3

All the code used is in the file matrix.cpp.
Since C++ primitives are used the compiler must be mpic++.

For ease of use a makefile is provided.

	make 		: will just compile the cpp into a executable file
	make clean 	: cleans the object files and creates a Results.txt, this is where the timing and verification results will be stored.
	make test	: will execute all the mpirun commands for all the combinations of n,p required
	make sub 	: will execute a series of qsub commands for flux which will produce a 3 executions of each n,p combination

Also pbs scripts are provided for the different number of processors ( p1.sh, p4.sh, p16.sh, p36.sh)