 #####################################################################
 # Example MPI C Makefile.
 #####################################################################
 
SRC = fsp
CC = g++
FILES = ${SRC}.cpp

default: ${SRC}

${SRC}: $(FILES)
	${CC} $(FILES) -o3 -o ${SRC} -fopenmp

clean:
	-rm -f ${SRC}

run: ${SRC}
	./${SRC}

sub: $(SRC)
	qsub p32.sh
	qsub p16.sh
	qsub p4.sh
	qsub p1.sh