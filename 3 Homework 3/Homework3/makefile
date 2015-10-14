 #####################################################################
 # Example MPI C Makefile.
 #####################################################################
 
SRC = matrix
CC = mpic++
NP = 36
ARGS = 1000

# HEADERS = $(SRC).h
OBJECTS = $(SRC).o

default: $(SRC)
# %.o: %.c $(HEADERS)
%.o: %.c
	$(CC) -c $< -o $@

$(SRC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(SRC)
	rm Results.txt
	touch Results.txt

run: $(SRC)
	mpirun -np $(NP) ./$(SRC) $(ARGS)

test: $(SRC)
	mpirun -np 1 ./$(SRC) 1000
	mpirun -np 1 ./$(SRC) 2000
	mpirun -np 4 ./$(SRC) 1000
	mpirun -np 4 ./$(SRC) 2000
	mpirun -np 16 ./$(SRC) 1000
	mpirun -np 16 ./$(SRC) 2000
	mpirun -np 36 ./$(SRC) 1000
	mpirun -np 36 ./$(SRC) 2000

sub: $(SRC)
	qsub p36.sh
	qsub p16.sh
	qsub p4.sh
	qsub p1.sh
	qsub p1.sh
	qsub p1.sh
	